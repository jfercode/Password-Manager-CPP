#ifndef LIBRARY_HPP
# define LIBRARY_HPP
    
    #pragma once

    #include <stdio.h>
    #include <iostream>
    #include <string>
    #include <vector>
    #include <sys/stat.h>

    #ifndef WIDTH
    # define WIDTH 1920
    #endif

    #ifndef HEIGTH
    # define HEIGTH 1080
    #endif

    // Ansi Colors
    #define BLACK "\033[30m"
    #define RED "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLUE "\033[34m"
    #define MAGENTA "\033[35m"
    #define CYAN "\033[36m"
    #define WHITE "\033[37m"
    #define RESET "\033[0m"

    // Qt includes
    #include <QApplication>
    #include <QtWidgets>
    #include <QMainWindow>
    #include <QLineEdit>
    #include <QtGui>

    // Forward QT library class declaration
    class QFont;
    class QLabel;
    class QDialog;
    class QWidget;
    class QLineEdit;
    class QtWidgets;
    class QMainWindow;
    class QVBoxLayout;
    class QPushButton;
    class QHBoxLayout;
    class QVBoxLayout;
    class QApplication;
    class QTableWidget;

    // UI classes
    #include "MainWindow.hpp"
    #include "LoginDialog.hpp"
    #include "NewUserDialog.hpp"

    // Crypto classes
    #include "CryptoManager.hpp"

    // Storage classes
    #include "SQLiteCipherDB.hpp"

    // App classes
    #include "AuthenticationManager.hpp"

    // Custom functions
    std::string ObtainCurrentTime();
    bool createDirectory(const std::string &dirPath);
    void PrintLog(std::ostream &oss, const std::string message, ...);

#endif