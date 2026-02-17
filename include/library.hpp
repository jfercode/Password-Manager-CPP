#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
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
    int id;
    std::string website;
    std::string username;
    std::string encrypted_password;
};

// Utility functions
std::string ObtainCurrentTime();
bool createDirectory(const std::string &dirPath);
void PrintLog(std::ostream &oss, const std::string message, ...);

#endif