#ifndef NEWUSERDIALOG_HPP
#define NEWUSERDIALOG_HPP

#include "library.hpp"
#include "SessionManager.hpp"

class NewUserDialog : public QDialog
{
    Q_OBJECT // Signals, slots and meta objects
        private:
            void setupUi();

            QLineEdit   *userEdit;
            QLineEdit   *passEdit;
            QLineEdit   *verifyEdit;
            QPushButton *loginBttn;
            QPushButton *cancelBttn;

    // User event functions
    private slots:
        void onLoginClicked();
        void onCancelClicked();

    public:
        explicit NewUserDialog(QWidget* parent = nullptr);
        ~NewUserDialog();
};

#endif