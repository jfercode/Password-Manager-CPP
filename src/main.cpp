#include "../include/library.hpp"

// Principal main
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    LoginDialog login;

    if (login.exec() == QDialog::Accepted)
    {
        MainWindow window; // Creates Main Window
        //  Here i need to start the loop
        return app.exec();
    }
    return (0);
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