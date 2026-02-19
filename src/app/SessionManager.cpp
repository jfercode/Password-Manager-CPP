#include "SessionManager.hpp"


// Initializates the singleton
SessionManager *SessionManager::_instance = nullptr;

SessionManager::SessionManager()
    : _masterPassword(""),
    _userSalt(""),
    _username(""),
    _isAuthenticated(false),
    _db(nullptr),
    _crypto(nullptr),
    _auth(nullptr)
{
    PrintLog(std::cout, CYAN "SessionManager" RESET " - Initialized");
}

SessionManager::~SessionManager()
{
    _masterPassword = "";
    _userSalt = "";
    _username = "";
    _isAuthenticated = false;
    _db = nullptr;
    _crypto = nullptr;
    _auth = nullptr;

    PrintLog(std::cout, CYAN "SessionManager" RESET " - Destroyed");
}

SessionManager *SessionManager::getInstance()
{
    if (_instance == nullptr)
        _instance = new SessionManager();
    
    return _instance;
}

// ============ SESSION DATA MANAGEMENT ============

void SessionManager::setUserId(int id)
{
    _user_id = id;
}

void SessionManager::setMasterPassword(const std::string &password)
{
    _masterPassword = password;
}

void SessionManager::setUserSalt(const std::string &salt)
{
    _userSalt = salt;
}

void SessionManager::setUsername(const std::string &username)
{
    _username = username;
}

void SessionManager::setAuthenticated(bool authenticated)
{
    _isAuthenticated = authenticated;
}

int SessionManager::getUserId() const
{
    return _user_id;
}

std::string SessionManager::getMasterPassword() const
{
    return _masterPassword;
}

std::string SessionManager::getUserSalt() const
{
    return _userSalt;
}

std::string SessionManager::getUsername() const
{
    return _username;
}

bool SessionManager::isAuthenticated() const
{
    return _isAuthenticated;
}

void SessionManager::clearSession()
{
    PrintLog(std::cout, CYAN "SessionManager" RESET " - Clearing session...");
    
    _user_id = 0;
    _masterPassword = "";
    _userSalt = "";
    _username = "";
    _isAuthenticated = false;
    
    PrintLog(std::cout, CYAN "SessionManager" GREEN " - Session cleared" RESET);
}

bool SessionManager::isValid() const
{
    // All conditions must be true
    bool valid = _isAuthenticated
                 && !_masterPassword.empty()
                 && !_userSalt.empty()
                 && !_username.empty();
    
    if (!valid)
    {
        PrintLog(std::cerr, RED "SessionManager - Invalid session" RESET);
    }
    
    return valid;
}

// ============ SERVICE MANAGEMENT ============

void SessionManager::initializeServices(SQLiteCipherDB *db, CryptoManager *crypto, AuthenticationManager *auth)
{
    _db = db;
    _crypto = crypto;
    _auth = auth;
    
    if (areServicesInitialized())
    {
        PrintLog(std::cout, CYAN "SessionManager" GREEN " - All services initialized successfully" RESET);
    }
    else
    {
        PrintLog(std::cerr, CYAN "SessionManager" RED " - WARNING: Not all services were initialized!" RESET);
    }
}

SQLiteCipherDB* SessionManager::getDatabase() const
{
    if (_db == nullptr)
    {
        PrintLog(std::cerr, RED "SessionManager - ERROR: Database service not initialized!" RESET);
    }
    return _db;
}

CryptoManager* SessionManager::getCryptoManager() const
{
    if (_crypto == nullptr)
    {
        PrintLog(std::cerr, RED "SessionManager - ERROR: Crypto service not initialized!" RESET);
    }
    return _crypto;
}

AuthenticationManager* SessionManager::getAuthenticationManager() const
{
    if (_auth == nullptr)
    {
        PrintLog(std::cerr, RED "SessionManager - ERROR: Authentication service not initialized!" RESET);
    }
    return _auth;
}

bool SessionManager::areServicesInitialized() const
{
    return (_db != nullptr) && (_crypto != nullptr) && (_auth != nullptr);
}
