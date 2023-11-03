#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unordered_set>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "database.hpp"
#include "password.hpp"
#include "server.hpp"

int main()
{
    database::Database db = database::Database("passwords.db");
    db.execute("CREATE TABLE passwords (password TEXT);");

    // generate and insert the passwords into the database
    std::unordered_set<std::string> passwords = password::generatePasswords(100, 20);
    for (const auto &password : passwords)
    {
        db.execute("INSERT INTO passwords (password) VALUES ('" + password + "');");
    }
    // test password
    passwords.insert("TestPass1&");
    db.execute("INSERT INTO passwords (password) VALUES ('TestPass1&');");
    db.printTable("passwords");

    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // initialize endpoints
    server::root(app);
    server::passwords(app, passwords);
    server::intersection(app, passwords);

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}