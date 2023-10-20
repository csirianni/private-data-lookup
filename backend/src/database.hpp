#include <sqlite3.h>

#ifndef DATABASE_H
#define DATABASE_H

namespace database
{
    /**
     * @brief Class containing a SQLite database connection
     *
     */
    class Database
    {
    public:
        /**
         * @brief Construct a new Database object
         *
         * @param file_path
         */
        Database(const std::string &file_path);
        /**
         * @brief
         *
         * @param command
         */
        void execute(const std::string &command);
        /**
         * @brief
         *
         */
        void printTable(const std::string &table);
        /**
         * @brief
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