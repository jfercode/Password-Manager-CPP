#ifndef LOGINDIALOG_HPP
#define LOGINDIALOG_HPP

#include "library.hpp"

class LoginDialog : public QDialog
{
    Q_OBJECT // Signals, slots and meta objects

    private:
        void setupUi();

        QLineEdit *userEdit;
        QLineEdit *passEdit;    
        QPushButton *loginBttn;
        QPushButton *cancelBttn;

    // User event functions
    private slots: 
        void onLoginClicked();
        void onCancelClicked();
    
    public:
        explicit LoginDialog(QWidget* parent = nullptr);
        
        ~LoginDialog();

};

#endif