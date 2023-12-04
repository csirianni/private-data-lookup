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

    SECTION("Rebuild the database of names, database should be empty")
    {
        database::Database db = database::Database(path, true);

        // callback function to check if the database is empty
        std::function<bool(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
            return count == 0;
        };

        // query the sqlite_schema table to check if the database is empty
        std::vector<bool> result = db.execute("SELECT COUNT(*) FROM sqlite_schema;", callback);
        CHECK(result.front() == true);

        db.close();
    }

    SECTION("Allow build")
    {
        // !build and file does not exist: runtime error
        REQUIRE_THROWS_AS(database::Database("random.db").close(), std::runtime_error);
        // !build and file exists: reuse the same file
        REQUIRE_NOTHROW(database::Database(path).close());
        // build and file exists: replace the file
        REQUIRE_NOTHROW(database::Database(path, true).close());
        // build and file does not exist: create a new file
        REQUIRE_NOTHROW(database::Database(path, true).close());
    }

    REQUIRE(std::remove(path.c_str()) == 0);
}