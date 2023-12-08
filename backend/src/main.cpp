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
    if (build)
    {   
        int offset = 1;
        // create password table
        db.execute("CREATE TABLE passwords (password TEXT);");

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
            // encode password before inserting into database
            db.execute("INSERT INTO passwords (password) VALUES ('" + crow::utility::base64encode(password, password.size()) + "');");
        }

        // create key table
        db.execute("CREATE TABLE secret (key TEXT);");

        // encode key b and insert into database
        db.execute("INSERT INTO secret (key) VALUES ('" + crow::utility::base64encode(std::string(reinterpret_cast<const char *>(b), crypto_core_ristretto255_SCALARBYTES), crypto_core_ristretto255_SCALARBYTES) + "');");
    }
    else
    {
        // error check if !build but passwords table does not exist in the file passed in
        std::function<bool(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
            return count;
        };

        // check if passwords and key table exists
        std::vector<bool> password_result = db.execute("SELECT COUNT(*) FROM sqlite_schema WHERE name = 'passwords';", callback);
        std::vector<bool> secret_result = db.execute("SELECT COUNT(*) FROM sqlite_schema WHERE name = 'secret';", callback);
        if (password_result.front() == 0 || secret_result.front() == 0) // no passwords or no key table exists
        {
            throw std::invalid_argument("Passwords and/or secret key table does not exist. Use --build to create a new database");
        }
    }

    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // initialize endpoints
    server::root(app);
    server::breachedPasswords(app, db);

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}