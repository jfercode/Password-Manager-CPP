#ifndef NEWUSERDIALOG_HPP
#define NEWUSERDIALOG_HPP

#include "library.hpp"

class NewUserDialog : public QDialog
{
    Q_OBJECT // Signals, slots and meta objects
        private:
            void setupUi();

    // User event functions
    private slots:

    public:
        explicit NewUserDialog(QWidget *parent = nullptr);
        ~NewUserDialog();
};

#endif