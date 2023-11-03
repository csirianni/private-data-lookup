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
     * @param app The crow server.
     */
    void root(crow::App<crow::CORSHandler> &app);

    /**
     * @brief Endpoint to show all breached passwords.
     *
     * @param app The crow server.
     * @param passwords The set of all breached passwords.
     */
    void allBreachedPasswords(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set);

    /**
     * @brief Endpoint to compute set intersection.
     *
     * @param app The crow server.
     * @param passwords The set of all breached passwords.
     */
    void checkPassword(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set);
}
#endif // SERVER_H