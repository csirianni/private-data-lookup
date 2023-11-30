#include "server.hpp"
#include "cryptography.hpp"

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
        std::vector<std::string> result;
        result.reserve(passwords.size());
        for (const auto &password : passwords) 
        {
            result.push_back(password);
        }
        response["passwords"] = result;
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

    void breachedPasswords(crow::App<crow::CORSHandler> &app, const std::vector<unsigned char*> &passwords, unsigned char *b)
    {
        CROW_ROUTE(app, "/breachedPasswords")
            .methods("POST"_method)([passwords, b](const crow::request &req)
                                    {
        crow::json::wvalue response;

        if (req.body.empty()) 
        { 
            response["status"] = "error";
            return response;
        }

        unsigned char user_password[req.body.size()+1];
        memcpy(user_password, req.body.c_str(), req.body.size()+1);
        std::cout << user_password << std::endl;
        std::cout << sizeof(user_password) << std::endl;

        unsigned char* password = cryptography::encryptUserPassword(user_password, b);
        response["status"] = "success";
        response["userPassword"] = password;
        // response["breachedPasswords"] = passwords;
        
        return response; });
    }

};