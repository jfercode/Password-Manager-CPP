#include "library.hpp"
#include "SQLiteCipherDB.hpp"
#include "CryptoManager.hpp"
#include "AuthenticationManager.hpp"
#include "InitializationManager.hpp"
#include "SessionManager.hpp"
#include "MainWindow.hpp"
#include "LoginDialog.hpp"

// Principal main
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  // Must be created FIRST
    
    try
    {
        // Initialize all services
        SQLiteCipherDB db;
        CryptoManager crypto;
        AuthenticationManager authM;
        InitializationManager init(&db, &authM);
        
        // Register services with SessionManager (central dependency injection point)
        SESSION->initializeServices(&db, &crypto, &authM);
        
        // Check if system is initialized (has admin user)
        bool systemInitialized = init.isSystemInitialized();
        
        // Always show LoginDialog (has both Login and Register tabs)
        LoginDialog *authDialog = new LoginDialog();
        
        if (!systemInitialized)
        {
            PrintLog(std::cout, YELLOW "System not initialized - Registration required" RESET);
            PrintLog(std::cout, GREEN "Showing LoginDialog with Register tab active" RESET);
            authDialog->setActiveTab(1);  // Show Register tab
        }
        else
        {
            PrintLog(std::cout, GREEN "Showing LoginDialog for existing user" RESET);
            authDialog->setActiveTab(0);  // Show Login tab (default)
        }
        
        // Execute the appropriate dialog
        if (authDialog && authDialog->exec() == QDialog::Accepted)
        {
            // Authentication successful - open main window
            PrintLog(std::cout, GREEN "Authentication successful! Opening MainWindow" RESET);
            MainWindow window;
            window.show();
            int result = app.exec();
            delete authDialog;
            return result;
        }
        else
        {
            // User canceled authentication
            PrintLog(std::cout, YELLOW "User canceled authentication" RESET);
            if (authDialog)
                delete authDialog;
            return (0);
        }
    }
    catch (const std::exception &e)
    {
        PrintLog(std::cerr, RED "Error during initialization: %s" RESET, e.what());
        QMessageBox::critical(nullptr, "Fatal Error", 
                            QString("Error during initialization:\n%1").arg(e.what()));
        return (1);
    }
}
