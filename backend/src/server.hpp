#include <string>
#include <unordered_set>
#include <vector>
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
    void passwords(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &passwords);

    /**
     * @brief Endpoint to compute set intersection.
     *
     * @param app The crow server.
     * @param passwords The set of all breached passwords.
     */
    void intersection(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &passwords);

    /**
     * @brief Endpoint to serve the user password and breached passwords raised to the power of b.
     *
     * @param app The crow server.
     * @param passwords The vector of all breached passwords.
     * @param b The secret key.
     */
    void breachedPasswords(crow::App<crow::CORSHandler> &app, const std::vector<std::string> &passwords, unsigned char *b);
}
#endif // SERVER_H