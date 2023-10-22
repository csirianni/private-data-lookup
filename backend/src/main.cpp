#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crow.h"
#include <sqlite3.h>
#include "database.hpp"
#include "passwords.hpp"

int main()
{   
    database::Database db = database::Database("passwords.db");
    db.execute("CREATE TABLE passwords (password TEXT);");
    db.execute("INSERT INTO passwords (password) VALUES ('chocolate1');");
    db.printTable("passwords");

    // declare crow application
    crow::SimpleApp app;

    // define endpoint at the root directory
    CROW_ROUTE(app, "/")
    ([]()
     {
        crow::json::wvalue response;
        response["status"] = "success";
        return response; });

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}