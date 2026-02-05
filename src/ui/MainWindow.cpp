#include "../../include/MainWindow.hpp"

/**
 * Default constuctor that initializate QMainWindow with 
 * custom functionality
 */
MainWindow::MainWindow(void)
{
    // Inicialization code here
    std::cout << "Initilizating UI..." << std::endl;
    m_window.show();
    
}

MainWindow::~MainWindow() {}