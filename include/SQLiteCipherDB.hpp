#ifndef SQLITECIPHERDB_HPP
#define SQLITECIPHERDB_HPP

// TODO: Add includes you need
// HINT: You need SQLite3
#include <string>
#include <sqlite3.h>
#include "library.hpp"

class SQLiteCipherDB
{
    private:
        // TODO: Add private members
        // HINT: You need a pointer to the database handle
        // HINT: You need a constant string for the database file path
        sqlite3 *db;
        std::string *dbPath;
        
        void setupDB(void);
    public:
        SQLiteCipherDB();
        ~SQLiteCipherDB();

        // Creates a new user in the DB
        bool createUser(
            const std::string &username,
            const std::string &passwordHash,
            const std::string &salt) const;

        // Get password hash by passing all data
        bool getUserHash(
            const std::string &username,
            std::string &hash,
            std::string &salt) const;

        // Check for a user in DB
        bool userExists(const std::string &username) const;
ate helper method to initialize the database schema
};

#endif