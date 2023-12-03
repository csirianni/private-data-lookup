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

int main()
{
    database::Database db = database::Database("passwords.db");
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
    for (const auto &password : passwords)
    {
        db.execute("INSERT INTO passwords (password) VALUES ('" + password + "');");
    }
    // test password
    db.execute("INSERT INTO passwords (password) VALUES ('TestPass1&');");

    std::function<std::string(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
    {
        return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
    };
    std::vector<std::string> test = db.execute("SELECT * FROM passwords;", callback);

    for (const auto &password : test)
    {
        printf("%s\n", password.c_str());
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