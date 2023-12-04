#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include "database.hpp"

TEST_CASE("Test Database class")
{
    std::string path = "test.db";
    std::ofstream file(path);
    REQUIRE(file.is_open());

    SECTION("Execute SELECT query and then open a new database with the same file, database should be the same")
    {
        // open a database
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

        // reopen the same database, build is false so the contents should be the same
        database::Database reopen_db = database::Database(path);

        // check if the names table is still there
        std::function<bool(sqlite3_stmt *)> callback_names_table = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
            return count > 0;
        };
        std::vector<bool> result_names_table = reopen_db.execute("SELECT COUNT(*) FROM sqlite_schema WHERE name = 'names';", callback_names_table);
        CHECK(result_names_table.front() == true);

        // check if the specific names are still there
        std::function<std::string(sqlite3_stmt *)> callback_each_name = [](sqlite3_stmt *stmt)
        {
            return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        };
        std::vector<std::string> result_each_name = reopen_db.execute("SELECT * FROM names;", callback_each_name);
        CHECK(result_each_name == names);

        db.close();
        reopen_db.close();
    }

    SECTION("Reopen the database"){
        // reopen the same database, build is false so the contents should be the same
        database::Database reopen_db = database::Database(path);

        std::vector<std::string> names = {"Cedric", "Stella", "Jessica"};

        // check if the names table is still there
        std::function<bool(sqlite3_stmt *)> callback_names_table = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
            return count > 0;
        };
        std::vector<bool> result_names_table = reopen_db.execute("SELECT COUNT(*) FROM sqlite_schema WHERE name = 'names';", callback_names_table);
        CHECK(result_names_table.front() == true);

        // check if the specific names are still there
        std::function<std::string(sqlite3_stmt *)> callback_each_name = [](sqlite3_stmt *stmt)
        {
            return std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        };
        std::vector<std::string> result_each_name = reopen_db.execute("SELECT * FROM names;", callback_each_name);
        CHECK(result_each_name == names);

        reopen_db.close();
    }

    SECTION("Rebuild the database of names, database should be empty")
    {
        database::Database db = database::Database(path, true);

        // callback function to capture the counts of executing the query
        std::function<bool(sqlite3_stmt *)> callback = [](sqlite3_stmt *stmt)
        {
            int count = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));

            // count == 0 since there is nothing in the database
            return (count == 0);
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