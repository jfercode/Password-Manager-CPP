#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "library.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT // Signals, slots and meta objects

    private:
        void setupUI();

        QPushButton *addBttn;
        QPushButton *refreshBttn;
        QPushButton *deleteBttn;
        QPushButton *logoutBttn;

    // User event functions
    private slots:
        void onClickAddPssBttn();
        void onClickDeleteBttn();
        void onClickLogoutBttn();

        void onViewPassword(int row);
        void onEditPassword(int row);
        void onDeletePassword(int row);

    public:
        explicit MainWindow(void);
        ~MainWindow();
};

#endif
