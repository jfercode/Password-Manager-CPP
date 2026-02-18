#ifndef SESSIONMANAGER_HPP
# define SESSIONMANAGER_HPP 

#include <string>
#include <iostream>

#include "library.hpp"
#include "SQLiteCipherDB.hpp"
#include "CryptoManager.hpp"
#include "AuthenticationManager.hpp"

// Singleton that serves as a central hub for all services and session data
// This is the main dependency injection point for the entire application
class SessionManager
{
    private:
        static SessionManager *_instance;

        // Session Data
        std::string _masterPassword;
        std::string _userSalt;
        std::string _username;
        bool _isAuthenticated;

        // Service pointers
        SQLiteCipherDB *_db;
        CryptoManager *_crypto;
        AuthenticationManager *_auth;

        SessionManager();

    public:
        // To prevent copy
        SessionManager(const SessionManager &) = delete;
        SessionManager& operator=(const SessionManager &) = delete;

        ~SessionManager();

        // Singleton -> obtain unique instance
        static SessionManager *getInstance();

        // SESSION DATA MANAGEMENT
        // Setters
        void setMasterPassword(const std::string &p);
        void setUserSalt(const std::string &s);
        void setUsername(const std::string &u);
        void setAuthenticated(bool a);

        // Getters
        std::string getMasterPassword(void) const;
        std::string getUserSalt(void) const;
        std::string getUsername(void) const;
        bool isAuthenticated(void) const;

        // Logout
        void clearSession();

        // Verify session is valid
        bool isValid() const;

        // SERVICE MANAGEMENT
        // Initialize all services
        void initializeServices(SQLiteCipherDB *db, CryptoManager *crypto, AuthenticationManager *auth);

        // Service Getters - use these to access services from anywhere
        SQLiteCipherDB* getDatabase() const;
        CryptoManager* getCryptoManager() const;
        AuthenticationManager* getAuthenticationManager() const;

        // Verify all services are initialized
        bool areServicesInitialized() const;
};

#define SESSION SessionManager::getInstance()

#endif