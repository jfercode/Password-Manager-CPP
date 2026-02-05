#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "library.hpp"

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QtGui>

class MainWindow
{

// Q_OBJECT // Signals, slots and meta objects

    private:
        void setupUI(); // Function that inits the UI
        QWidget m_window;

    // User event functions
    private slots:

    public:
        explicit MainWindow(void); // constructor explicito que inicializará la clase qmainwindow
        ~MainWindow();
};

#endif
