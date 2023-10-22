#include <sqlite3.h>

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
        sqlite3 *db;
        bool is_closed;
    };
}
#endif // DATABASE_H