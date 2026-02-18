#include "AuthenticationManager.hpp"
#include "SessionManager.hpp"

AuthenticationManager::AuthenticationManager()
{
    PrintLog(std::cout, CYAN "Authentication Manager" RESET " - initialized" RESET);
}

AuthenticationManager::~AuthenticationManager() {}

bool    AuthenticationManager::authenticateUser(const std::string &username, const std::string &password) const
{
    // Search for a user in the DB
    PrintLog(std::cout, CYAN "Authentication Manager" RESET " - authenticating user %s...", username.c_str());

    std::string stored_hash, stored_salt;
    if (!SESSION->getDatabase()->getUserHash(username, stored_hash, stored_salt))
        return false;
    
    // Verify Password
    int res = SESSION->getCryptoManager()->verifyPassword(password, stored_hash, stored_salt);
    if (res)
        PrintLog(std::cout, CYAN "Authentication Manager" RESET " - user %s authenticated", username.c_str());
    else
        PrintLog(std::cout, CYAN "Authentication Manager" RED " - user %s not authenticated" RESET, username.c_str());
    return res;
}

bool    AuthenticationManager::registerNewUser(const std::string &username, const std::string &password, bool isMaster) const
{
    PrintLog(std::cout, CYAN "Authentication Manager" RESET " - registing user %s...", username.c_str());
    if (SESSION->getDatabase()->userExists(username))
    {
        PrintLog(std::cout, CYAN "Authentication Manager" RESET " -  user %s already exist", username.c_str());
        return false;
    }
    
    // hash the password
    auto [hash, salt] = SESSION->getCryptoManager()->hashPassword(password);
    int res = SESSION->getDatabase()->createUser(username, hash, salt, isMaster);

    if (res)
        PrintLog(std::cout, CYAN "Authentication Manager" RESET " - user %s created", username.c_str());
    else
        PrintLog(std::cout, CYAN "Authentication Manager" RED " - fails to create user %s" RESET, username.c_str());
    return res;
}
