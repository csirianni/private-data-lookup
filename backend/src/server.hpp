#include <string.h>
#include <unordered_set>
#include "crow.h"
#include "crow/middlewares/cors.h"

#ifndef SERVER_H
#define SERVER_H

namespace server
{
    void root(crow::App<crow::CORSHandler> &app);

    void allBreachedPasswords(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set);

    /**
     * @brief
     * Note we are not using const-reference because crow is not
     * @param app
     * @param password_set
     *
     */
    // void checkPassword(crow::App<crow::CORSHandler> &app, const std::unordered_set<std::string> &password_set);
}
#endif // SERVER_H