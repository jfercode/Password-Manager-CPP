#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "library.hpp"

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>

class MainWindow : public QMainWindow
{

Q_OBJECT // Signals, slots and meta objects

    private:
        void setupUI(); // Function that inits the UI

    // User event functions
    private slots:

    public:
        explicit MainWindow(QWidget *parent = null); // constructor explicito que inicializará la clase qmainwindow
        ~MainWindow();
};

#endif
