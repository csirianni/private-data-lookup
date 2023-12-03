#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include "database.hpp"

TEST_CASE("Test Database class")
{
    std::string path = "test.db";
    std::ofstream file(path);
    REQUIRE(file.is_open());

    SECTION("Execute SELECT query")
    {
        database::Database db = database::Database(path);
        REQUIRE_NOTHROW(db.execute("CREATE TABLE names (name TEXT);"));

        std::vector<std::string> names = {"Cedric", "Stella", "Jessica"};
        for (const auto &name : names)
        {
            REQUIRE_NOTHROW(db.execute("INSERT INTO names (name) VALUES ('" + name + "');"));
        }

        std::function<std::string(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
        {
            return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        };
        std::vector<std::string> result = db.execute("SELECT * FROM names;", callback);
        CHECK(result == names);

        db.close();
    }

    SECTION("Allow rebuild")
    {
        // try to rebuild a file that doesn't exist
        REQUIRE_THROWS_AS(database::Database("random.db", true).close(), std::runtime_error);
        // rebuild the file
        REQUIRE_NOTHROW(database::Database(path, true).close());
        // reuse the same file
        REQUIRE_NOTHROW(database::Database(path).close());
    }

    REQUIRE(std::remove(path.c_str()) == 0);
}