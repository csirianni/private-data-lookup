#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "database.hpp"
#include "password.hpp"
#include <unordered_set>

/*
    Testing the server can run and passwords will be posted.
*/
void test_api(crow::App<crow::CORSHandler> *curr_app)
{
    curr_app->validate(); // Used to make sure all the route handlers are in order.

    {
        crow::request req;
        crow::response res;

        // Test the endpoint for running the server
        req.url = "/";
        curr_app->handle(req, res);
        if (res.code == 200)
        {
            boost_swap_impl::cout << "Server test: success connecting to endpoint to run the server \n\n";
        }
        else
        {
            boost_swap_impl::cerr << "Set Intersection Test error: " + boost_swap_impl::to_string(res.code) + "\n\n";
        }

        // Test the endpoint for compute set intersection
        req.url = "/passwords";
        req.method = "POST"_method;
        req.add_header("Access-Control-Allow-Headers", "*");
        req.add_header("Content-Type", "application/json");
        req.body = "TestPass1&";

        curr_app->handle(req, res); // res will contain a code of 200, and a response body of "success"
        if (res.code == 200)
        {
            boost_swap_impl::cout << "Set Intersection Test: success connecting to endpoint to compute set intersection \n\n";
        }
        else
        {
            boost_swap_impl::cerr << "Set Intersection Test error: " + boost_swap_impl::to_string(res.code) + "\n\n";
        }
    }
}

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
    password_set.insert("TestPass1&"); // test password
    db.execute("INSERT INTO passwords (password) VALUES ('TestPass1&');");
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

    test_api(&app);

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
    db.close();
}