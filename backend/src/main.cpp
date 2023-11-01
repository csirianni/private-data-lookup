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
    db.execute("INSERT INTO passwords (password) VALUES ('TestPass1&');"); // test password
    db.printTable("passwords");

    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    // clang formatting off
    cors
        .global()
        .headers("*")
        .methods("POST"_method);
    // clang formatting on

    CROW_ROUTE(app, "/") // endpoint to establish server connection
    ([]()
     {  crow::json::wvalue response;
        response["status"] = "success";
        response["data"] = "server is now running";
        return response; });
    CROW_ROUTE(app, "/allBreachedPasswords") // temporary endpoint for debugging, shows all passwords
    ([&password_set]()
     {  crow::json::wvalue response;
        for (auto const &breached_password : password_set) // for each password in the set, add it as a json key 
        {
            response[breached_password];
        }
        return response; });
    CROW_ROUTE(app, "/passwords") // endpoint to compute set intersection
        .methods("POST"_method)([&password_set](const crow::request &req)
                                {
        crow::json::wvalue response;
        std::string userPassword = req.body; // user's password passed in the post request
        const bool inBreachedPasswords = password_set.find(userPassword) != password_set.end(); 
        if (inBreachedPasswords){ // password is in the breached password set
            response["status"] = "fail";
        } else { // password is not in the breached password set
            response["status"] = "success";
        }
        return response; });

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}