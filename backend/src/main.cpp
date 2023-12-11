#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unordered_set>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "database.hpp"
#include "password.hpp"
#include "server.hpp"
#include "sodium.h"
#include "cryptography.hpp"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::debug);

    if (argc < 2 || argc > 3)
    {
        printf("Usage: %s <database file> [--build]\n", argv[0]);
        return 1;
    }

    bool build = false;
    if (argc == 3)
    {
        if (std::string(argv[2]) == "--build")
        {
            build = true;
        }
        else
        {
            printf("Usage: %s <database file> [--build]\n", argv[0]);
            return 1;
        }
    }

    // check if file does not end in .db
    std::string file_path = argv[1];
    if (file_path.find(".db") != file_path.length() - 3)
    {
        throw std::invalid_argument("File does not end in .db. Use the correct file extension to open the database.");
    }

    database::Database db = database::Database(argv[1], build);
    // TODO: store this data in server because same file with different offset will have different passwords
    const size_t offset = 1;
    spdlog::info("Password offset: {}", offset);

    // limit the number of leaked bytes to 4
    if (offset > 4)
    {
        throw std::invalid_argument("Offset cannot be greater than 4.");
    }

    // build the database
    if (build)
    {
        // create all tables from 0 to (2^8)^offset-1
        for (int i = 0; i < std::pow(std::pow(2, 8), offset); i++)
        {
            db.execute("CREATE TABLE `" + std::to_string(i) + "` (password TEXT);");
        }

        // generate and insert the passwords into the database
        std::unordered_set<std::string> passwords = password::generatePasswords(100, 20);
        passwords.insert("TestPass1&");
        passwords.insert("ChocolateCake1!");

        // 1. generate secret key b
        unsigned char b[crypto_core_ristretto255_SCALARBYTES];
        crypto_core_ristretto255_scalar_random(b);

        // 2. encrypt each password with b (and hash to point)
        std::vector<std::string> encrypted_passwords = cryptography::encrypt(passwords, b, offset);

        // 3. insert into database
        for (const auto &password : encrypted_passwords)
        {
            // determine which table to insert into based on leaked byte
            std::string encoded_byte = crow::utility::base64encode(password.substr(0, offset), offset);
            std::string table_num = std::to_string(static_cast<unsigned int>(encoded_byte[0]));
            std::string raw_password = password.substr(offset, password.size() - offset);
            // encode password before inserting into database
            db.execute("INSERT INTO `" + table_num + "` (password) VALUES ('" + crow::utility::base64encode(raw_password, raw_password.size()) + "');");
        }

        // create key table
        db.execute("CREATE TABLE secret (key TEXT);");
        // encode key b and insert into database
        db.execute("INSERT INTO secret (key) VALUES ('" + crow::utility::base64encode(std::string(reinterpret_cast<const char *>(b), crypto_core_ristretto255_SCALARBYTES), crypto_core_ristretto255_SCALARBYTES) + "');");

        // create offset table and insert offset
        db.execute("CREATE TABLE offset (key INTEGER);");
        db.execute("INSERT INTO offset (key) VALUES (" + std::to_string(offset) + ");");
    }
    else
    {
        // error check if !build but secret table does not exist in the file passed in
        std::function<bool(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
            return count;
        };

        // check if key and offset table exists
        std::vector<bool> secret_key = db.execute("SELECT COUNT(*) FROM sqlite_schema WHERE name = 'secret';", callback);
        std::vector<bool> offset_key = db.execute("SELECT COUNT(*) FROM sqlite_schema WHERE name = 'offset';", callback);
        if (secret_key.front() == 0 || offset_key.front() == 0) // no secret key or no offset key table exists
        {
            throw std::invalid_argument("Secret key table or offset key table does not exist. Use --build to create a new database");
        }
    }
    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // initialize endpoints
    server::root(app);
    server::breachedPasswords(app, db, offset);

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}