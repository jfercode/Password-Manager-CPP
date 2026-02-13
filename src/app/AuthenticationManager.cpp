#include "AuthenticationManager.hpp"
#include "CryptoManager.hpp"
#include "SQLiteCipherDB.hpp"

AuthenticationManager::AuthenticationManager(const CryptoManager *cry, const SQLiteCipherDB *dB)
{
    PrintLog(std::cout, CYAN "Authentication Manager" RESET " - initialized" RESET);
    crypto = cry;
    db = dB;
}

AuthenticationManager::~AuthenticationManager() {}

bool    AuthenticationManager::authenticateUser(const std::string &username, const std::string &password) const
{
    // Search for a user in the DB
    PrintLog(std::cout, CYAN "Authentication Manager" RESET " - authenticating user %s...", username.c_str());

    std::string stored_hash, stored_salt;
    if (!db->getUserHash(username, stored_hash, stored_salt))
        return false;
    
    // Verify Password
    int res = crypto->verifyPassword(password, stored_hash, stored_salt);
    if (res)
        PrintLog(std::cout, CYAN "Authentication Manager" RESET " - user %s authenticated", username.c_str());
    else
        PrintLog(std::cout, CYAN "Authentication Manager" RED " - user %s not authenticated" RESET, username.c_str());
    return res;
}

bool    AuthenticationManager::registerNewUser(const std::string &username, const std::string &password) const
{
    PrintLog(std::cout, CYAN "Authentication Manager" RESET " - registing user %s...", username.c_str());
    if (db->userExists(username))
    {
        PrintLog(std::cout, CYAN "Authentication Manager" RESET " -  user %s already exist", username.c_str());
        return false;
    }

    // hash the password
    auto [hash, salt] = crypto->hashPassword(password);
    int res = db->createUser(username, hash, salt);
    
    if (res)
        PrintLog(std::cout, CYAN "Authentication Manager" RESET " - user %s created", username.c_str());
    else
        PrintLog(std::cout, CYAN "Authentication Manager" RED " - fails to create user %s" RESET, username.c_str());
    return res;
}
