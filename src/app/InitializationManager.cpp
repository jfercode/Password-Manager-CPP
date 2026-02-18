#include "InitializationManager.hpp"

InitializationManager::InitializationManager(const SQLiteCipherDB *database, const AuthenticationManager *auth)
    : db(database), authM(auth)
{
    PrintLog(std::cout, CYAN "InitializationManager" RESET " - Initializing...");
}

InitializationManager::~InitializationManager()
{
    // PrintLog(std::cout, CYAN "InitializationManager" RESET " - Destroyed");
}

bool InitializationManager::isSystemInitialized() const
{
    if (!db)
    {
        PrintLog(std::cerr, RED "Error: Database not initialized" RESET);
        return false;
    }
    
    bool hasMaster = db->hasMasterUser();
    if (hasMaster)
        PrintLog(std::cout, CYAN "InitializationManager" RESET " - System is " GREEN "initialized" RESET);
    else
        PrintLog(std::cout, CYAN "InitializationManager" RESET " - System is " YELLOW "NOT initialized" RESET);
    
    return hasMaster;
}

int InitializationManager::getRequiredDialogType() const
{
    if (isSystemInitialized())
    {
        PrintLog(std::cout, CYAN "InitializationManager" RESET " - Returning LoginDialog (existing user)");
        return 1; // LoginDialog
    }
    else
    {
        PrintLog(std::cout, CYAN "InitializationManager" RESET " - Returning NewUserDialog (first setup)");
        return 0; // NewUserDialog
    }
}
