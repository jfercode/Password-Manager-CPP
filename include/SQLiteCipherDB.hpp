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
        sqlite3 *db;
        std::string dbPath;
        
        void setupDB(void);
        bool findDataBasePath();
    public:
        SQLiteCipherDB();
        ~SQLiteCipherDB();

        // Creates a new user in the DB
        bool createUser(
            const std::string &username,
            const std::string &passwordHash,
            const std::string &salt,
            bool isMaster) const;

        // Get password hash by passing all data
        bool getUserHash(
            const std::string &username,
            std::string &hash,
            std::string &salt) const;

        // Check for a user in DB
        bool userExists(const std::string &username) const;

        // Check for master user (first login)
        bool hasMasterUser() const;

        // Check if username given is masterUser
        bool isMasterUser(const std::string &username) const;
};

#endif