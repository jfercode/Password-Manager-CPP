#ifndef INITIALIZATIONMANAGER_HPP
#define INITIALIZATIONMANAGER_HPP

#include "library.hpp"
#include "SQLiteCipherDB.hpp"
#include "AuthenticationManager.hpp"

class InitializationManager
{
    private:
        const SQLiteCipherDB *db;
        const AuthenticationManager *authM;

    public:
        InitializationManager(const SQLiteCipherDB *database, const AuthenticationManager *auth);
        ~InitializationManager();

        // Check if the system is initialized (has master user)
        bool isSystemInitialized() const;

        // Get the type of dialog needed: 0 = NewUserDialog, 1 = LoginDialog
        int getRequiredDialogType() const;
};

#endif
