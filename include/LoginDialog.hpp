#ifndef LOGINDIALOG_HPP
#define LOGINDIALOG_HPP

#include "library.hpp"
#include "SessionManager.hpp"

class LoginDialog : public QDialog
{
    Q_OBJECT // Signals, slots and meta objects

    private:
        // Tab widget
        QTabWidget *tabWidget;
        
        // Login Tab widgets
        QLineEdit *loginUserEdit;
        QLineEdit *loginPassEdit;
        QCheckBox *showPassCheckbox;
        QPushButton *loginBttn;
        
        // Register Tab widgets
        QLineEdit *regUserEdit;
        QLineEdit *regPassEdit;
        QLineEdit *regConfirmPassEdit;
        QProgressBar *passwordStrengthBar;
        QLabel *strengthLabel;
        QPushButton *registerBttn;
        
        // Shared button
        QPushButton *cancelBttn;
        
        // Setup methods
        void setupUi();
        void setupLoginTab();
        void setupRegisterTab();
        
        // Helper methods
        int calculatePasswordStrength(const std::string &password);
        bool validatePassword(const std::string &password);

    // User event functions
    private slots: 
        void onLoginClicked();
        void onRegisterClicked();
        void onCancelClicked();
        void onShowPasswordToggled(bool checked);
        void onPasswordChanged(const QString &pass);
    
    public:
        explicit LoginDialog(QWidget* parent = nullptr);
        ~LoginDialog();
        
        // Set which tab to show initially (0 = Login, 1 = Register)
        void setActiveTab(int tabIndex);

};

#endif