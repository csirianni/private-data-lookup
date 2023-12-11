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

    // offset constant
    int offset = 1;

    // create all tables from 0 to (2^8)^offset-1
    for (int i = 0; i < std::pow(std::pow(2, 8), offset); i++)
    {
        std::string query_str = "table" + std::to_string(i);
        db.execute("CREATE TABLE " + query_str + " (password TEXT);");
        spdlog::info("Table key: {}", query_str);
    }

    // create a mock password set
    std::unordered_set<std::string> passwords = {"TestPass1&", "ChocolateCake1!", "LoveMyDogs3$"};

    // 1. generate secret key b
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);

    // 2. encrypt each password with b (and hash to point)
    std::vector<std::string> encrypted_passwords = cryptography::encrypt(passwords, b, offset);

    // 3. insert into database
    for (const auto &password : encrypted_passwords)
    {
        // determine which table to insert into based on leaked byte
        std::string encoded_byte = crow::utility::base64encode(password.substr(0, offset), offset);
        unsigned int table_num = static_cast<int>(encoded_byte[0]);
        std::string table_str = std::to_string(table_num);
        std::string query_str = "table" + table_str;
        spdlog::info("Password Table: {}", query_str);

        std::string raw_password = password.substr(offset, password.size() - offset);
        spdlog::info("Password str for above: {}", raw_password);

        // encode password before inserting into database
        db.execute("INSERT INTO " + query_str + " (password) VALUES ('" + crow::utility::base64encode(raw_password, raw_password.size()) + "');");
    }

    // create key table
    db.execute("CREATE TABLE secret (key TEXT);");

    // encode key b and insert into database
    db.execute("INSERT INTO secret (key) VALUES ('" + crow::utility::base64encode(std::string(reinterpret_cast<const char *>(b), crypto_core_ristretto255_SCALARBYTES), crypto_core_ristretto255_SCALARBYTES) + "');");

    // initialize endpoints
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

        // encrypt user password with b (and hash to point)
        std::string user_request = "TestPass1&";
        std::string encrypted_password = cryptography::hashAndEncryptPassword(user_request, b, offset);
        req.body = crow::utility::base64encode(encrypted_password, encrypted_password.size());

        app.handle(req, res);

        auto body = nlohmann::json::parse(res.body);
        CHECK(body["status"] == "success");

        // check correct bucket is returned
        REQUIRE(body["breachedPasswords"].is_array());
        std::vector<std::string> breached_bucket = body["breachedPasswords"].get<std::vector<std::string>>();
        CHECK(breached_bucket.size() == 1);

        // check breached password encoding
        std::string breached_password = breached_bucket[0];
        CHECK(breached_password[breached_password.size() - 1] == '=');

        // check user password encoded
        std::string user_password = body["userPassword"];
        CHECK(!user_password.empty());
        CHECK(user_password.back() == '=');

        CHECK(res.code == 200);
    }
    REQUIRE(std::remove(path.c_str()) == 0);
}