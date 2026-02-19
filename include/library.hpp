#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <stdexcept>
#include <sys/stat.h>
#include <sqlite3.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

// Qt includes
#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QTabWidget>
#include <QCheckBox>
#include <QProgressBar>
#include <QFont>

// Ansi Colors and constants
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"

#define WIDTH 800
#define HEIGHT 600

// Data structures
struct Password
{
    int id;                         // Unique ID in DB
    std::string website;            // ej: "Gmail", "Facebook"
    std::string username;           // ej: "username@example.com"
    std::string encrypted_password; // Ciphertext in hex
    std::string iv;                 // IV in Hexadecimal
    std::string created_at;         // Creation timestamp

    Password()
        : id(0), website(""), username(""), encrypted_password(""), iv(""), created_at("") {}

    Password(int _id, const std::string &_website,
             const std::string &_username,
             const std::string &_encrypted,
             const std::string &_iv,
             const std::string &_created)
                : id(_id), website(_website), username(_username),
                encrypted_password(_encrypted), iv(_iv),
                created_at(_created) {}
};

// Utility functions
std::string ObtainCurrentTime();
bool createDirectory(const std::string &dirPath);
void PrintLog(std::ostream &oss, const std::string message, ...);

// SessionManager macro for easy access to the singleton instance
#define SESSION SessionManager::getInstance()

#endif