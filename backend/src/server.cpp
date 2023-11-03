#include "server.hpp"

namespace server
{
    void root(crow::App<crow::CORSHandler> &app)
    {
        CROW_ROUTE(app, "/")
        ([]()
         {  crow::json::wvalue response;
        response["status"] = "success";
        response["data"] = "server is now running";
        return response; });
    }

    void allBreachedPasswords(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set)
    {
        CROW_ROUTE(app, "/allBreachedPasswords")
        ([password_set]()
         {  crow::json::wvalue response;
        for (auto const breached_password : password_set) // for each password in the set, add it as a json key 
        {
            response[breached_password];
        }
        return response; });
    }

    // void checkPassword(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set)
    // {
    //     CROW_ROUTE(app, "/passwords") // endpoint to compute set intersection
    //         .methods("POST"_method)([&password_set](const crow::request &req)
    //                                 {
    //     crow::json::wvalue response;
    //     std::string userPassword = req.body; // user's password passed in the post request
    //     const bool inBreachedPasswords = *password_set.find(userPassword) != *password_set.end(); 
    //     if (inBreachedPasswords){ // password is in the breached password set
    //         response["status"] = "fail";
    //     } else { // password is not in the breached password set
    //         response["status"] = "success";
    //     }
    //     return response; });
    // }

};