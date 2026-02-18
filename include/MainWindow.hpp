#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "library.hpp"
#include "SessionManager.hpp"
#include "AddPasswordDialog.hpp"
#include "EditPasswordDialog.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT // Signals, slots and meta objects

    private:
        QTableWidget *passwordTable;
        
        void setupUI();
        void updateUI();
        
        QPushButton *addBttn;
        QPushButton *refreshBttn;
        QPushButton *logoutBttn;

    // User event functions
    private slots:
        void onClickAddPssBttn();
        void onClickLogoutBttn();

        void onViewPassword(int id);
        void onEditPassword(int id);
        void onDeletePassword(int id);

    public:
        explicit MainWindow();
        ~MainWindow();
        
        void    updateUi(void);
    };

#endif
