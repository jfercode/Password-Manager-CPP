#ifndef ADDPASSDIALOG_HPP
# define ADDPASSDIALOG_HPP

#include "library.hpp"

class SQLiteCipherDB;

class AddPasswordDialog : public QDialog
{
    Q_OBJECT // Signals, slots and meta objects

    private:
        void setupUi();
        
        SQLiteCipherDB *_db;
        
        QLineEdit *webEdit;
        QLineEdit *userEdit;
        QLineEdit *passEdit;    

        QPushButton *saveBttn;
        QPushButton *cancelBttn;

    // User event functions
    private slots: 
        void onSaveClicked();
        void onCancelClicked();
    
    public:
        explicit AddPasswordDialog(QWidget* parent = nullptr, SQLiteCipherDB *db = nullptr);
        
        ~AddPasswordDialog();

};

#endif