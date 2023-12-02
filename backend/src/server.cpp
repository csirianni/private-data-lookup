#include "server.hpp"
#include "cryptography.hpp"

namespace
{
    std::vector<std::string> encodePasswords(const std::vector<std::string> &passwords)
    {
        std::vector<std::string> result;
        result.reserve(passwords.size());

        for (const auto &password : passwords)
        {
            result.push_back(crow::utility::base64encode(password, password.size()));
        }

        return result;
    }
}

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

    void breachedPasswords(crow::App<crow::CORSHandler> &app, const std::vector<std::string> &passwords, unsigned char *b)
    {
        CROW_ROUTE(app, "/breachedPasswords")
            .methods("POST"_method)([passwords, b](const crow::request &req)
                                    {
        crow::json::wvalue response;
        std::string user_password = req.body.data();
        if (user_password.empty()) 
        { 
            response["status"] = "error";
            return response;
        }

        std::string encrypted_password = cryptography::encryptPassword(crow::utility::base64decode(user_password, user_password.size()), b);
        // std::string encrypted_password = cryptography::encryptPassword("TestPass1&", b);

        response["status"] = "success";
        response["userPassword"] = crow::utility::base64encode(encrypted_password, encrypted_password.size());
        response["breachedPasswords"] = encodePasswords(passwords);
        
        return response; });
    }
};