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

    database::Database db = database::Database(argv[1], build);
    if (build)
    {

        db.execute("CREATE TABLE passwords (password TEXT);");
    }

    // generate and insert the passwords into the database
    std::unordered_set<std::string> passwords = password::generatePasswords(100, 20);
    passwords.insert("TestPass1&");
    passwords.insert("ChocolateCake1!");

    // 1. generate secret key b
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);

    // 2. encrypt each password with b (and hash to point)
    std::vector<std::string> encrypted_passwords = cryptography::encrypt(passwords, b);

    // 3. insert into database
    for (const auto &password : encrypted_passwords)
    {
        // encode password before inserting into database
        db.execute("INSERT INTO passwords (password) VALUES ('" + crow::utility::base64encode(password, password.size()) + "');");
    }

    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // initialize endpoints
    server::root(app);
    server::breachedPasswords(app, db, b);

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}