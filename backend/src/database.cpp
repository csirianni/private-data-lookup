#include <filesystem>
#include <sqlite3.h>
#include "database.hpp"
#include "spdlog/spdlog.h"

namespace
{
    // https://stackoverflow.com/questions/31146713/sqlite3-exec-callback-function-clarification
    static int printRow(void *unused, int width, char **data, char **attributes)
    {
        for (int i = 0; i < width; i++)
        {
            printf("%s = %s\n", attributes[i], data[i] ? data[i] : "NULL");
        }
        printf("\n");
        return 0;
    }
}

namespace database
{
    Database::Database(const std::string &file_path, bool build) : is_closed_(false)
    {
        // build the database:: if file exists, remove the file
        if (build && std::filesystem::exists(file_path))
        {
            if (std::remove(file_path.c_str()) != 0)
            {
                throw std::runtime_error("Unable to remove file");
            }
        }
        // use existing database: if file does not exist, throw an error
        else if (!build && !std::filesystem::exists(file_path))
        {
            throw std::runtime_error("File does not exist. Use --build to create a new database");
        }

        // open the database (will create a file path if it does not exist)
        int result = sqlite3_open_v2(file_path.c_str(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (result != SQLITE_OK) // database could not be opened
        {
            const char *error_msg = sqlite3_errmsg(db_);
            throw std::runtime_error(std::string("SQLite error: ") + error_msg);
        }
        else
        {
            spdlog::info("Database created successfully");
            spdlog::info("SQLite version: {}", sqlite3_libversion());
        }
    }

    void Database::execute(const std::string &command)
    {
        int result = sqlite3_exec(db_, command.c_str(), NULL, NULL, NULL);
        if (result != SQLITE_OK)
        {
            const char *error_msg = sqlite3_errmsg(db_);
            fprintf(stderr, "SQLite error: %s\n", error_msg);
        }
    }

    void Database::printTable(const std::string &table)
    {
        const std::string query = "SELECT * FROM " + table + ";";
        int result = sqlite3_exec(db_, query.c_str(), printRow, NULL, NULL);
        if (result != SQLITE_OK)
        {
            const char *error_msg = sqlite3_errmsg(db_);
            fprintf(stderr, "SQLite error: %s\n", error_msg);
        }
    }

    void Database::close()
    {
        int result = sqlite3_close(db_);
        if (result != SQLITE_OK)
        {
            const char *error_msg = sqlite3_errmsg(db_);
            fprintf(stderr, "SQLite error: %s\n", error_msg);
        }
        is_closed_ = true;
    }

    Database::~Database()
    {
        if (!is_closed_)
        {
            int result = sqlite3_close(db_);
            if (result != SQLITE_OK)
            {
                const char *error_msg = sqlite3_errmsg(db_);
                fprintf(stderr, "SQLite error: %s\n", error_msg);
                std::abort();
            }
        }
    }
}