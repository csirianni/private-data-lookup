#include "server.hpp"
#include "sqlite3.h"

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

    void breachedPasswords(crow::App<crow::CORSHandler> &app, database::Database &db, unsigned char *b)
    {
        CROW_ROUTE(app, "/breachedPasswords")
            .methods("POST"_method)([&db, b](const crow::request &req)
                                    {
        crow::json::wvalue response;
        std::string user_password = req.body.data();
        if (user_password.empty()) 
        { 
            response["status"] = "error";
            return response;
        }

        std::string encrypted_password = cryptography::encryptPassword(crow::utility::base64decode(user_password, user_password.size()), b);

        std::function <std::string(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt) {
            return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        };
        std::vector<std::string> result = db.execute("SELECT * FROM passwords;", callback);

        response["status"] = "success";
        response["userPassword"] = crow::utility::base64encode(encrypted_password, encrypted_password.size());
        response["breachedPasswords"] = result;
        
        return response; });
    }
};