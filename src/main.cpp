#include "library.hpp"
#include "InitializationManager.hpp"

// Principal main
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  // Must be created FIRST
    
    try
    {
        // Initialize database and authentication
        SQLiteCipherDB db;
        CryptoManager crypto;
        AuthenticationManager authM(&crypto, &db);
        InitializationManager init(&db, &authM);
        
        // Check if system is initialized (has admin user)
        bool systemInitialized = init.isSystemInitialized();
        
        QDialog *authDialog = nullptr;
        
        if (systemInitialized)
        {
            // System initialized - show LoginDialog for existing users
            PrintLog(std::cout, GREEN "Showing LoginDialog for existing user" RESET);
            authDialog = new LoginDialog(nullptr, &authM);
        }
        else
        {
            // System not initialized - show NewUserDialog to create first admin
            PrintLog(std::cout, GREEN "Showing NewUserDialog for first admin setup" RESET);
            authDialog = new NewUserDialog(nullptr, &authM);
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

// // TESTING SQL
// int main(void)
// {
//     SQLiteCipherDB db; // Test constructor
//     std::cout << "Database opened successfully!" << std::endl;

//     // Test createUser
//     bool ok = db.createUser("testuser", "hash123", "salt456");
//     if (ok)
//         std::cout << "User created!" << std::endl;
//     else
//         std::cout << "User creation failed!" << std::endl;

//     // Test userExists
//     bool exists = db.userExists("testuser");
//     std::cout << "User exists: " << (exists ? "YES" : "NO") << std::endl;

//     // Test getUserHash
//     std::string h, s;
//     bool found = db.getUserHash("testuser", h, s);
//     if (found)
//     {
//         std::cout << "Hash: " << h << std::endl;
//         std::cout << "Salt: " << s << std::endl;
//     }
//     return 0;
// }