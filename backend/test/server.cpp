#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "server.hpp"
#include "password.hpp"
#include "cryptography.hpp"
#include "sodium.h"

TEST_CASE("Test endpoints return response code 200")
{
    // enable CORS
    crow::App<crow::CORSHandler> app;

    // customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // create a mock password set
    std::unordered_set<std::string> passwords = password::generatePasswords(3, 12);

    // initialize endpoints
    server::root(app);
    server::passwords(app, passwords);
    server::intersection(app, passwords);

    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);
    std::vector<std::string> encrypted_passwords = cryptography::encrypt(passwords, b);
    server::breachedPasswords(app, encrypted_passwords, b);

    // check that all the route handlers were created
    app.validate();

    crow::request req;
    crow::response res;

    SECTION("Root")
    {
        req.url = "/";

        app.handle(req, res);
        CHECK(res.code == 200);
    }

    SECTION("Passwords")
    {
        req.url = "/passwords";

        app.handle(req, res);
        CHECK(res.code == 200);
    }

    SECTION("Intersection")
    {
        req.url = "/intersection";
        req.method = "POST"_method;
        req.add_header("Access-Control-Allow-Headers", "*");
        req.add_header("Content-Type", "application/json");
        req.body = "TestPass1&";

        app.handle(req, res);
        CHECK(res.code == 200);
    }

    SECTION("Breached Passwords")
    {
        req.url = "/breachedPasswords";
        req.method = "POST"_method;
        req.add_header("Access-Control-Allow-Headers", "*");
        req.add_header("Content-Type", "application/json");
        req.body = "TestPass1&";

        app.handle(req, res);
        CHECK(res.code == 200);
    }
}
