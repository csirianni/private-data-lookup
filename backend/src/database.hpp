#include <sqlite3.h>
#include <vector>
#include <functional>

#ifndef DATABASE_H
#define DATABASE_H

namespace database
{
    /**
     * @brief Class containing a SQLite database connection
     *
     * @warning Rule of Three/Five not currently defined for this class.
     */
    class Database
    {
    public:
        /**
         * @brief Construct a new Database object
         *
         * @warning If a file already exists at the provided path, it is replaced.
         * @param file_path The path for the .db file.
         */
        Database(const std::string &file_path);

        /**
         * @brief Execute the provided SQL command.
         *
         * @param command The command to be executed.
         */
        void execute(const std::string &command);

        // TODO: add documentation
        template <typename T>
        std::vector<T> execute(const std::string &command, std::function<T(sqlite3_stmt *)> callback)
        {
            sqlite3_stmt *stmt;
            if (sqlite3_prepare_v2(db_, command.c_str(), -1, &stmt, NULL) != SQLITE_OK)
            {
                const char *error_msg = sqlite3_errmsg(db_);
                fprintf(stderr, "SQLite error: %s\n", error_msg);
            }
            std::vector<T> result;
            // TODO: vector reserve
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                result.push_back(callback(stmt));
            }
            sqlite3_finalize(stmt);
            return result;
        }

        /**
         * @brief Print the rows of the provided table.
         *
         * @param table The table to print
         */
        void printTable(const std::string &table);

        /**
         * @brief Close the connection with the SQLite database.
         *
         */
        void close();

        /**
         * @brief Destroy the Database object
         *
         */
        ~Database();

    private:
        sqlite3 *db_;
        bool is_closed_;
    };
}
#endif // DATABASE_H