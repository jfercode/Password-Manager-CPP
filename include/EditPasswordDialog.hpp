#ifndef EDITPASSDIALOG_HPP
# define EDITPASSDIALOG_HPP

#include "library.hpp"
#include "SQLiteCipherDB.hpp"

class EditPasswordDialog : public QDialog
{
    Q_OBJECT // Signals, slots and meta objects

    private:
        void setupUi();
        
        SQLiteCipherDB *_db;
        int _passwordId;
        
        QLineEdit *webEdit;
        std::string webStr;
        QLineEdit *userEdit;
        std::string userStr;
        QLineEdit *passEdit;
        std::string passStr;

        QPushButton *saveBttn;
        QPushButton *cancelBttn;

    // User event functions
    private slots: 
        void onSaveClicked();
        void onCancelClicked();
    
    public:
        explicit EditPasswordDialog(QWidget* parent = nullptr, SQLiteCipherDB *db = nullptr, int id = 0);
        
        ~EditPasswordDialog();
};

#endif