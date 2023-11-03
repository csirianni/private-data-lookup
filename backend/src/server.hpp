#include <string.h>
#include <unordered_set>
#include "crow.h"
#include "crow/middlewares/cors.h"

#ifndef SERVER_H
#define SERVER_H

namespace server
{
    /**
     * @brief Endpoint to check the server is running.
     *
     * @param app - crow server 
     * @note crow does not use const-reference
     */
    void root(crow::App<crow::CORSHandler> &app);

    /**
     * @brief Endpoint to show all breached passwords
     *
     * @param app - crow server
     * @param password_set - set of all breached passwords
     * @note crow does not use const-reference
     */
    void allBreachedPasswords(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set);

    /**
     * @brief Endpoint to compute set intersection.
     * 
     * @param app - crow server
     * @param password_set  - set of all breached passwords
     * @note crow does not use const-reference
     */
    void checkPassword(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set);
}
#endif // SERVER_H