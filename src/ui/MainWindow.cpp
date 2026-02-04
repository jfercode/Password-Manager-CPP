#include "../../include/MainWindow.hpp"

/**
 * Default constuctor that initializate QMainWindow with 
 * custom functionality
 */
MainWindow::MainWindow(QWidget *parent = null) : QMainWindow(parent)
{
    // Inicialization code here
    std::cout << "Initilizating UI..." << std::endl;
}

