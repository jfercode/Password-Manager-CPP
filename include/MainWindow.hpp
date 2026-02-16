#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "library.hpp"
class SQLiteCipherDB;

class MainWindow : public QMainWindow
{
    Q_OBJECT // Signals, slots and meta objects

    private:
        SQLiteCipherDB *_db;
        QTableWidget *passwordTable;
        
        void setupUI();
        void updateUI();
        
        QPushButton *addBttn;
        QPushButton *refreshBttn;
        QPushButton *deleteBttn;
        QPushButton *logoutBttn;

    // User event functions
    private slots:
        void onClickAddPssBttn();
        void onClickLogoutBttn();

        void onViewPassword(int id);
        void onEditPassword(int id);
        void onDeletePassword(int id);

    public:
        explicit MainWindow(SQLiteCipherDB *db);
        ~MainWindow();
        
        void    updateUi(void);
    };

#endif
