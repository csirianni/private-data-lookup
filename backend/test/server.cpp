#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "server.hpp"
#include "password.hpp"

TEST_CASE("Test endpoints return response code 200")
{
    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // Create a mock password set
    std::unordered_set<std::string> password_set = password::generatePasswords(3, 12);

    // Server endpoints
    server::root(app);
    server::allBreachedPasswords(app, password_set);
    server::checkPassword(app, password_set);

    // Check that all the route handlers were created
    app.validate();

    // define response and request
    crow::request req;
    crow::response res;

    SECTION("root")
    {
        req.url = "/";

        app.handle(req, res);
        CHECK(res.code == 200);
    }

    SECTION("breached passwords")
    {
        req.url = "/passwords";
        req.method = "POST"_method;
        req.add_header("Access-Control-Allow-Headers", "*");
        req.add_header("Content-Type", "application/json");
        req.body = "TestPass1&";

        app.handle(req, res);
        CHECK(res.code == 200);
    }

    SECTION("set intersection")
    {
        req.url = "/passwords";
        req.method = "POST"_method;
        req.add_header("Access-Control-Allow-Headers", "*");
        req.add_header("Content-Type", "application/json");
        req.body = "TestPass1&";

        app.handle(req, res);
        CHECK(res.code == 200);
    }
}
