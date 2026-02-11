#ifndef AUTHENTICATORMANAGER_HPP
#define AUTHENTICATORMANAGER_HPP

#include <string>

// Forward declarations
class CryptoManager;
class SQLiteCipherDB;

class AuthenticationManager
{
    private:
        const CryptoManager *crypto;
        const SQLiteCipherDB *db;        
        
    public:
        AuthenticationManager(const CryptoManager *cry, const SQLiteCipherDB *dB);
        ~AuthenticationManager();

        //  Authenticate existed user
        bool    authenticateUser(const std::string &username, const std::string &password) const;
        
        //  Register a new user into the system 
        bool    registerNewUser(const std::string &username, const std::string &password) const;
};

#endif