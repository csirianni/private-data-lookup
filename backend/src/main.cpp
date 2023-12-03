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

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        printf("Usage: %s <database file> [--rebuild]\n", argv[0]);
        return 1;
    }

    bool rebuild = false;
    if (argc == 3)
    {
        if (std::string(argv[2]) == "--rebuild")
        {
            rebuild = true;
        }
        else
        {
            printf("Usage: %s <database file> [--rebuild]\n", argv[0]);
            return 1;
        }
    }

    database::Database db = database::Database(argv[1], rebuild);
    db.execute("CREATE TABLE passwords (password TEXT);");

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
    // TODO: changet this to encrypted_passwords after encoding
    for (const auto &password : passwords)
    {
        db.execute("INSERT INTO passwords (password) VALUES ('" + password + "');");
    }

    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // initialize endpoints
    server::root(app);
    server::breachedPasswords(app, encrypted_passwords, b);

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}