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

    void passwords(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &passwords)
    {
        CROW_ROUTE(app, "/passwords")
        ([passwords]()
         {  crow::json::wvalue response;
        // TODO: change this to be a list
        for (auto const &breached_password : passwords) 
        {
            response[breached_password];
        }
        return response; });
    }

    void intersection(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &passwords)
    {
        CROW_ROUTE(app, "/intersection")
            .methods("POST"_method)([passwords](const crow::request &req)
                                    {
        crow::json::wvalue response;

        std::string user_password = req.body;
        if (user_password.empty()) 
        { 
            response["status"] = "error";
            return response;
        }

        const bool is_breached = passwords.find(user_password) != passwords.end();
        if (is_breached)
        { 
            response["status"] = "fail";
        }
        else 
        { 
            response["status"] = "success";
        }

        return response; });
    }

};