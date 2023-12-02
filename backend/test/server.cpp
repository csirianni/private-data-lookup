#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "server.hpp"
#include "password.hpp"
#include "cryptography.hpp"
#include "sodium.h"
#include <nlohmann/json.hpp>

TEST_CASE("Test endpoints using handler")
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

    SECTION("Breached Passwords")
    {
        req.url = "/breachedPasswords";
        req.method = "POST"_method;
        req.add_header("Access-Control-Allow-Headers", "*");
        req.add_header("Content-Type", "application/json");
        req.body = "TestPass1&";

        app.handle(req, res);

        auto body = nlohmann::json::parse(res.body);
        CHECK(body["status"] == "success");

        REQUIRE(body["breachedPasswords"].is_array());
        std::vector<std::string> breached_passwords = body["breachedPasswords"].get<std::vector<std::string>>();
        CHECK(breached_passwords.size() == 3);
        for (const auto &breached_password : breached_passwords)
        {
            // encoded password should end with '='
            CHECK(breached_password.back() == '=');
        }

        std::string user_password = body["userPassword"];
        CHECK(!user_password.empty());
        CHECK(user_password.back() == '=');

        CHECK(res.code == 200);
    }
}
