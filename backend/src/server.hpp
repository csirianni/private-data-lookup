#include <string>
#include <unordered_set>
#include <vector>

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "cryptography.hpp"
#include "database.hpp"

#ifndef SERVER_H
#define SERVER_H

namespace server {

/**
 * @brief Endpoint to check the server is running.
 *
 * @param app The crow server.
 */
void root(crow::App<crow::CORSHandler> &app);

/**
 * @brief Endpoint to serve the user password and breached passwords raised to
 * the power of b.
 *
 * @param app The crow server.
 * @param db The database of all breached passwords.
 * @param offset The number of bytes to leak.
 */
void breachedPasswords(crow::App<crow::CORSHandler> &app,
                       database::Database &db, size_t offset = 0);
}  // namespace server
#endif  // SERVER_H
