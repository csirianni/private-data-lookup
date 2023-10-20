#include <filesystem>
#include <sqlite3.h>
#include "database.hpp"

namespace
{
    static int printData(void *NotUsed, int argc, char **argv, char **azColName)
    {
        for (int i = 0; i < argc; i++)
        {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\n");
        return 0;
    }
}

namespace database
{
    void initializeDatabase(const char *file)
    {
        if (std::filesystem::exists(file))
        {
            std::remove(file);
        }

        sqlite3 *db;
        int result = sqlite3_open_v2("passwords.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        // TODO: improve this error handling
        if (result != SQLITE_OK)
        {
            const char *errorMsg = sqlite3_errmsg(db);
            fprintf(stderr, "SQLite error: %s\n", errorMsg);
        }

        // TODO: investigate IF NOT EXISTS
        const char *table = "CREATE TABLE passwords (password TEXT);";
        result = sqlite3_exec(db, table, NULL, NULL, NULL);
        if (result != SQLITE_OK)
        {
            const char *errorMsg = sqlite3_errmsg(db);
            fprintf(stderr, "SQLite error: %s\n", errorMsg);
        }

        const char *insertion = "INSERT INTO passwords (password) VALUES ('chocolate1');";
        result = sqlite3_exec(db, insertion, NULL, NULL, NULL);
        if (result != SQLITE_OK)
        {
            const char *errorMsg = sqlite3_errmsg(db);
            fprintf(stderr, "SQLite error: %s\n", errorMsg);
        }

        const char *selection = "SELECT * FROM passwords";
        result = sqlite3_exec(db, selection, printData, NULL, NULL);
        if (result != SQLITE_OK)
        {
            const char *errorMsg = sqlite3_errmsg(db);
            fprintf(stderr, "SQLite error: %s\n", errorMsg);
        }

        sqlite3_close(db);
    }
}