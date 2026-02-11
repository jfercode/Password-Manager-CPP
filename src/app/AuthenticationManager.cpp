#include "AuthenticationManager.hpp"
#include "CryptoManager.hpp"
#include "SQLiteCipherDB.hpp"

AuthenticationManager::AuthenticationManager(const CryptoManager *cry, const SQLiteCipherDB *dB) {
    crypto = cry;
    db = dB;
}

AuthenticationManager::~AuthenticationManager() {}

bool    AuthenticationManager::authenticateUser(const std::string &username, const std::string &password) const
{
    // Search for a user in the DB
    std::string stored_hash, stored_salt;
    if (!db->getUserHash(username, stored_hash, stored_salt))
        return false;
    
    // Verify Password
    return crypto->verifyPassword(password, stored_hash, stored_salt);
}


bool    AuthenticationManager::registerNewUser(const std::string &username, const std::string &password) const
{
    if (db->userExists(username))
        return false;

    // hash the password
    auto [hash, salt] = crypto->hashPassword(password);

    return db->createUser(username, hash, salt);
}
