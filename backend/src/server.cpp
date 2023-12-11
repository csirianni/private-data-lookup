#include "server.hpp"
#include "sqlite3.h"
#include "spdlog/spdlog.h"
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

    void breachedPasswords(crow::App<crow::CORSHandler> &app, database::Database &db, size_t offset)
    {
        CROW_ROUTE(app, "/breachedPasswords")
            .methods("POST"_method)([&db, offset](const crow::request &req)
                                    {
        crow::json::wvalue response;
        std::string user_password = req.body.data();
        if (user_password.empty()) 
        { 
            spdlog::error("Empty user password");
            response["status"] = "error";
            return response;
        }

        // get all passwords from database
        std::function <std::string(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt) {
            return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        };

        // get the table num corresponding to the user password leaked byte
        std::string decoded_password = crow::utility::base64decode(user_password, user_password.size());
        std::string encoded_leaked_byte = crow::utility::base64encode(decoded_password.substr(0, offset), offset);
        std::string table_num = "table" + std::to_string(static_cast<unsigned int>(encoded_leaked_byte[0]));

        // get all passwords from the table corresponding to the user password leaked byte
        std::vector<std::string> breached_passwords = db.execute("SELECT * FROM " + table_num + ";", callback);

        // get b secret key from database
        std::string encoded_b = db.execute("SELECT * FROM secret;", callback)[0];
        // decode secret key b
        std::string decoded_b = crow::utility::base64decode(encoded_b, encoded_b.size());

        // copy secret key b into an unsigned char* 
        unsigned char *b = (unsigned char *)decoded_b.data();

        // encrypt user password
        std::string encrypted_password = cryptography::encryptPassword(crow::utility::base64decode(user_password, user_password.size()), b, offset);
        response["status"] = "success";
        response["userPassword"] = crow::utility::base64encode(encrypted_password, encrypted_password.size());
        response["breachedPasswords"] = breached_passwords;
        
        return response; });
    }
};