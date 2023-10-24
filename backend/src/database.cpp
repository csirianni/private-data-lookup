#include <filesystem>
#include <sqlite3.h>
#include "database.hpp"

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
    Database::Database(const std::string &file_path) : is_closed_(false)
    {
        if (std::filesystem::exists(file_path))
        {
            std::remove(file_path.c_str());
        }

        int result = sqlite3_open_v2(file_path.c_str(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (result != SQLITE_OK)
        {
            const char *error_msg = sqlite3_errmsg(db_);
            fprintf(stderr, "SQLite error: %s\n", error_msg);
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
            }
            std::abort();
        }
    }
}