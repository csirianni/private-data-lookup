#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "database.hpp"
#include "password.hpp"

int main()
{
    database::Database db = database::Database("passwords.db");
    db.execute("CREATE TABLE passwords (password TEXT);");

    // generate and insert the passwords into the database
    std::unordered_set<std::string> password_set = password::generatePasswords(100, 20);
    for (const auto &password : password_set)
    {
        db.execute("INSERT INTO passwords (password) VALUES ('" + password + "');");
    }
    db.execute("INSERT INTO passwords (password) VALUES ('chocolate1');");
    db.printTable("passwords");

    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method, "GET"_method);

    CROW_ROUTE(app, "/")
    ([]()
     {
        crow::json::wvalue response;
        response["status"] = "success";
        response["data"] = "none";
        return response; });

    CROW_ROUTE(app, "/<string>")
    ([](boost_swap_impl::string a)
     {
        crow::json::wvalue response;
        response["status"] = "success";
        response["data"] = a;
        return response; });

    CROW_ROUTE(app, "/cors")
    ([]()
     { return "Check Access-Control-Allow-Origin header"; });

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}