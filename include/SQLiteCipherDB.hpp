#ifndef SQLITECIPHERDB_HPP
#define SQLITECIPHERDB_HPP

// TODO: Add includes you need
// HINT: You need SQLite3
#include <string>
#include <sqlite3.h>
#include <vector>
#include "library.hpp"

// Structure to represent a stored password
struct Password {
    int id;
    std::string website;
    std::string username;
    std::string encrypted_password;
};

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

        // Add a new password to the database
        bool addPassword(
            const std::string &website,
            const std::string &username,
            const std::string &encrypted_password) const;

        // Get all passwords from the database
        std::vector<Password> getAllPasswords() const;

        // Get a specific password by ID
        bool getPassword(int id, Password &password) const;

        // Update a password by ID
        bool updatePassword(
            int id,
            const std::string &website,
            const std::string &username,
            const std::string &encrypted_password) const;

        // Delete a password by ID
        bool deletePassword(int id) const;

        // Get the number of stored passwords
        int getPasswordCount() const;
};

#endif