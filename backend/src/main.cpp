#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "database.hpp"
#include "password.hpp"
#include <unordered_set>

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

    // clang formatting off
    cors
        .global()
        .headers("*")
        .methods("POST"_method, "GET"_method);
    // clang formatting on

    CROW_ROUTE(app, "/")
    ([]()
     {
        crow::json::wvalue response;
        response["status"] = "success";
        response["data"] = "none";
        return response; });

    CROW_ROUTE(app, "/passwords")
        .methods("POST"_method)([&password_set](const crow::request &req)
                                {
        crow::json::wvalue response;
        const boost_swap_impl::string userPassword = req.body;
        const bool inBreachedPasswords = password_set.find(userPassword) != password_set.end();

        if (inBreachedPasswords){ // password is in the set
            response["status"] = "fail";
        } else {
            response["status"] = "success";
        }
        response["data"] = userPassword;
        for (auto const &breached_password : password_set)
        {
            response[breached_password];
        }
        return response; });

    CROW_ROUTE(app, "/cors")
    ([]()
     { return "Check Access-Control-Allow-Origin header"; });

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}