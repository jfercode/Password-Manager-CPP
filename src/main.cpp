#include "../include/library.hpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    LoginDialog login;

    if (login.exec() == QDialog::Accepted)
    {
        MainWindow window;  // Creates Main Window
        //  Here i need to start the loop
        return app.exec();
    }
    return (0);
}