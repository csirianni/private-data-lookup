#include <string>
#include <unordered_set>
#include <vector>
#include "database.hpp"
#include "cryptography.hpp"
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
     * @brief Endpoint to serve the user password and breached passwords raised to the power of b.
     *
     * @param app The crow server.
     * @param db The database of all breached passwords.
     * @param b The secret key.
     */
    void breachedPasswords(crow::App<crow::CORSHandler> &app, database::Database &db, unsigned char *b);
}
#endif // SERVER_H