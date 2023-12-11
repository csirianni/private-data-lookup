#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "server.hpp"
#include "password.hpp"
#include "cryptography.hpp"
#include "sodium.h"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

TEST_CASE("Test endpoints using handler")
{
    // enable CORS
    crow::App<crow::CORSHandler> app;

    // customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global().headers("*").methods("POST"_method);

    // initialize endpoints
    server::root(app);

    // open a database file
    std::string path = "test.db";
    std::ofstream file(path);
    REQUIRE(file.is_open());

    // create the database, with tables passwords and secret
    database::Database db = database::Database(path, true);
    REQUIRE_NOTHROW(db.execute("CREATE TABLE passwords (password TEXT);"));
    REQUIRE_NOTHROW(db.execute("CREATE TABLE secret (key TEXT);"));

    // create a mock password set
    std::unordered_set<std::string> passwords = {"TestPass1&", "ChocolateCake1!", "LoveMyDogs3$"};

    // 1. generate secret key b
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);

    // 2. encrypt each password with b (and hash to point)
    const size_t offset = 1;
    std::vector<std::string> encrypted_passwords = cryptography::encrypt(passwords, b, offset);

    // 3. insert into database
    for (const auto &password : encrypted_passwords)
    {
        // encode password before inserting into database
        db.execute("INSERT INTO passwords (password) VALUES ('" + crow::utility::base64encode(password, password.size()) + "');");
    }

    // encode key b and insert into database
    db.execute("INSERT INTO secret (key) VALUES ('" + crow::utility::base64encode(std::string(reinterpret_cast<const char *>(b), crypto_core_ristretto255_SCALARBYTES), crypto_core_ristretto255_SCALARBYTES) + "');");

    server::breachedPasswords(app, db, offset);

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
            // offset determines the amount of padding appended to the end of the password
            switch (offset % 3)
            {
            case 0:
                // password is padded with single '='
                CHECK(breached_password[breached_password.size() - 1] == '=');
                CHECK(breached_password[breached_password.size() - 2] != '=');
                break;
            case 1:
                // password is not padded
                CHECK(breached_password[breached_password.size() - 1] != '=');
                CHECK(breached_password[breached_password.size() - 2] != '=');
                break;
            case 2:
                // password is padded with two '='
                CHECK(breached_password[breached_password.size() - 1] == '=');
                CHECK(breached_password[breached_password.size() - 2] == '=');
                break;
            }
        }
        std::string user_password = body["userPassword"];
        CHECK(!user_password.empty());
        CHECK(user_password.back() == '=');

        CHECK(res.code == 200);
    }
    REQUIRE(std::remove(path.c_str()) == 0);
}