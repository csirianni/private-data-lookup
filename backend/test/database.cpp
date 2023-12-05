#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include "database.hpp"

std::string path = "test.db";
std::string failpath = "random.db";
std::ofstream file(path);

TEST_CASE("Test Database class")
{
    REQUIRE(file.is_open());

    SECTION("Create, insert, and select queries")
    {
        // open database
        database::Database db = database::Database(path);
        REQUIRE_NOTHROW(db.execute("CREATE TABLE names (name TEXT);"));

        // insert names into the database
        std::vector<std::string> names = {"Cedric", "Stella", "Jessica"};
        for (const auto &name : names)
        {
            REQUIRE_NOTHROW(db.execute("INSERT INTO names (name) VALUES ('" + name + "');"));
        }

        // check that the names are in the database
        std::function<std::string(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
        {
            return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        };
        std::vector<std::string> result = db.execute("SELECT * FROM names;", callback);
        CHECK(result == names);

        db.close();
    }

    SECTION("Reopening a database has persistent storage")
    {
        // reopen database
        database::Database reopen_db = database::Database(path);
        std::vector<std::string> names = {"Cedric", "Stella", "Jessica"};

        // check if the names table is still there
        std::function<bool(sqlite3_stmt *)> callback_names_table = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
            return count;
        };
        std::vector<bool> result_names_table = reopen_db.execute("SELECT COUNT(*) FROM sqlite_schema WHERE name = 'names';", callback_names_table);
        CHECK(result_names_table.front() == 1);

        // check if the previously inserted names are still there
        std::function<std::string(sqlite3_stmt *)> callback_each_name = [](sqlite3_stmt *stmt)
        {
            return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        };
        std::vector<std::string> result_each_name = reopen_db.execute("SELECT * FROM names;", callback_each_name);
        CHECK(result_each_name.size() == 3);
        CHECK(result_each_name == names);

        reopen_db.close();
    }

    SECTION("Rebuild existing database")
    {
        // setting build to true for an existing database will rebuild the database
        database::Database db = database::Database(path, true);

        // callback function to capture the counts of executing the query
        std::function<bool(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));

            // there is nothing in the database
            return count;
        };

        // query the sqlite_schema table to check that the database is empty
        std::vector<bool> result = db.execute("SELECT COUNT(*) FROM sqlite_schema;", callback);
        CHECK(result.front() == 0);

        db.close();
    }

    SECTION("Build flags")
    {
        // !build and file does not exist: runtime error
        REQUIRE_THROWS_AS(database::Database(failpath).close(), std::invalid_argument);
        // !build and file exists: reopen
        REQUIRE_NOTHROW(database::Database(path).close());
        // build and file exists: rebuild
        REQUIRE_NOTHROW(database::Database(path, true).close());
        // build and file does not exist: create
        REQUIRE_NOTHROW(database::Database(failpath, true).close());
    }

    SECTION("Teardown")
    {
        REQUIRE(std::remove(path.c_str()) == 0);
        REQUIRE(std::remove(failpath.c_str()) == 0);
    }
}