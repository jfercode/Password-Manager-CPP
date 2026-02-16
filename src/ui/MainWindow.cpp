#include "../../include/MainWindow.hpp"

// MainWindow Constructor
MainWindow::MainWindow(SQLiteCipherDB *db) : _db(db)
{
    // Window Setup
    setWindowTitle("Password Manager - Secure Storage");
    // TODO: Add icon
    // setWindowIcon(QIcon(":/icon route"));

    // Set up Ui
    PrintLog(std::cout, YELLOW "Main Window" RESET " - Initialazing UI...");
    setupUI();

    // TODO Conect bttns to functions here
    PrintLog(std::cout, YELLOW "Main Window" RESET " - Establishing buttons connection...");
    connect(addBttn, &QPushButton::clicked, this, &MainWindow::onClickAddPssBttn);
    connect(logoutBttn, &QPushButton::clicked, this, &MainWindow::onClickLogoutBttn);

    PrintLog(std::cout, YELLOW "Main Window" RESET " - Showing UI...");
    show();
}

// MainWindow Destructor
MainWindow::~MainWindow() {}

// Sets up the full layout of this window
void MainWindow::setupUI()
{
    resize(WIDTH, HEIGTH); // TODO: make widthxheight tests
    setMinimumSize(WIDTH / 2, HEIGTH / 2);

    //  Main container (central widget)
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main Vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // ============ HEADER SECTION ============ //
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // Title label
    QLabel *tittleLabel = new QLabel("Your Passwords", this);
    QFont tittleFont = tittleLabel->font();
    tittleFont.setPointSize(16);
    tittleFont.setBold(true);
    tittleLabel->setFont(tittleFont);

    // Search Box
    // QLineEdit *searchBox = new QLineEdit(this);
    // searchBox->setPlaceholderText("Search passwords...");
    // searchBox->setMaximumWidth(250);

    headerLayout->addWidget(tittleLabel);
    headerLayout->addStretch();
    // headerLayout->addWidget(searchBox);

    mainLayout->addWidget(tittleLabel);

    // ============ TABLE SECTION ============ //
    passwordTable = new QTableWidget(this);
    passwordTable->setColumnCount(4);
    passwordTable->setHorizontalHeaderLabels({"Website", "Username", "Password", "Actions"});
    passwordTable->horizontalHeader()->setStretchLastSection(false);
    passwordTable->setColumnWidth(0, 200);
    passwordTable->setColumnWidth(1, 200);
    passwordTable->setColumnWidth(2, 150);
    passwordTable->setColumnWidth(3, 150);
    passwordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    passwordTable->setSelectionMode(QAbstractItemView::SingleSelection);
    passwordTable->setAlternatingRowColors(true);

    // Action Layout - CREATE A CONTAINER WIDGET
    QWidget *actionWidget = new QWidget(this);
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(2, 2, 2, 2);
    actionLayout->setSpacing(5);

    mainLayout->addWidget(passwordTable);

    // ============ BUTTONS SECTION ============ //
    QHBoxLayout *bttnLayout = new QHBoxLayout();
    bttnLayout->setSpacing(8);

    addBttn = new QPushButton("Add a new Password", this);
    addBttn->setMinimumWidth(150);

    deleteBttn = new QPushButton("Delete Password", this);
    deleteBttn->setMinimumWidth(150);

    logoutBttn = new QPushButton("Logout", this);
    logoutBttn->setMinimumWidth(150);

    bttnLayout->addWidget(addBttn);
    bttnLayout->addWidget(deleteBttn);
    bttnLayout->addStretch();
    bttnLayout->addWidget(logoutBttn);

    mainLayout->addLayout(bttnLayout);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);

    // Update Ui with information from db
    updateUi();
}

// Update Main window interface
void MainWindow::updateUi()
{
    // Clean current passwordTable
    if (passwordTable->rowCount() > 0)
    {
        while (passwordTable->rowCount() > 0)
            passwordTable->removeRow(0);
    }

    // Obtain all passwords from db
    std::vector<Password> passwords = _db->getAllPasswords();

    // Iterate for each password in the vector
    for (const auto &pwd : passwords)
    {
        // New row for each iteration
        int row = passwordTable->rowCount();
        passwordTable->insertRow(row);

        // Add all items
        passwordTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(pwd.website)));
        PrintLog(std::cout, BLUE "DEBUGING" RESET " pwd.website -> %s", pwd.website.c_str());
        passwordTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(pwd.username)));

        QLineEdit *pwdEdit = new QLineEdit(this);
        pwdEdit->setText(QString::fromStdString(pwd.encrypted_password));
        pwdEdit->setEchoMode(QLineEdit::Password); // ← Show "*"
        pwdEdit->setReadOnly(true);
        pwdEdit->setProperty("passwordId", pwd.id); // save ID for later
        passwordTable->setCellWidget(row, 2, pwdEdit);

        // Create all buttons action with widgets
        QWidget *actionWidget = new QWidget(this);
        QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(2, 2, 2, 2);
        actionLayout->setSpacing(5);

        // Crete thre buttons for password in db: view, edit and delete
        QPushButton *viewBtn = new QPushButton("👁", this);
        viewBtn->setMaximumWidth(40);
        QPushButton *editBtn = new QPushButton("✏️", this);
        editBtn->setMaximumWidth(40);
        QPushButton *deleteBtn = new QPushButton("🗑️", this);
        deleteBtn->setMaximumWidth(40);

        // Add buttons to layout
        actionLayout->addWidget(viewBtn);
        actionLayout->addWidget(editBtn);
        actionLayout->addWidget(deleteBtn);
        actionLayout->addStretch();

        // Put widget into table
        passwordTable->setCellWidget(row, 3, actionWidget);

        // Connect butons with functions
        connect(viewBtn, &QPushButton::clicked, this, [this, pwd]()
                { this->onViewPassword(pwd.id); });
        connect(editBtn, &QPushButton::clicked, this, [this, pwd]()
                { this->onEditPassword(pwd.id); });
        connect(deleteBtn, &QPushButton::clicked, this, [this, pwd]()
                { this->onDeletePassword(pwd.id); });
    }
}

// Buttons handle
void MainWindow::onClickAddPssBttn()
{
    PrintLog(std::cout, MAGENTA "Add Password Button" RESET " - Adding a new password...");

    // Create add password dialog
    AddPasswordDialog dialog(this, _db);

    // Show dialog
    if (dialog.exec() == QDialog::Accepted)
    {
        PrintLog(std::cout, GREEN "Password dialog accepted" RESET);
        this->updateUi();
    }
}

void MainWindow::onClickLogoutBttn()
{
    PrintLog(std::cout, MAGENTA "Logout Button" RESET " - Loging out...");
    // Confirm logout
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Logout", "Are you sure you want to logout?", QMessageBox::Yes | QMessageBox::No);

    // Close window
    if (reply == QMessageBox::Yes)
        this->close();
}

void MainWindow::onViewPassword(int id)
{
    PrintLog(std::cout, MAGENTA "View Password" RESET " for ID %d", id);
    
    // Find the file with the id
    for (int row = 0; row < passwordTable->rowCount(); row++)
    {
        QLineEdit *pwdEdit = qobject_cast<QLineEdit*>(passwordTable->cellWidget(row, 2));
        if (pwdEdit && pwdEdit->property("passwordId").toInt() == id)
        {
            // Toggle between password (hidden) and normal (view)
            if (pwdEdit->echoMode() == QLineEdit::Password)
                pwdEdit->setEchoMode(QLineEdit::Normal);
            else
                pwdEdit->setEchoMode(QLineEdit::Password);
            break;
        }
    }
}

void MainWindow::onEditPassword(int id)
{
    PrintLog(std::cout, MAGENTA "Edit Password" RESET " for ID %d ", id);

    // Get current password
    // Password pwd;
    // _db->getPassword(id, pwd);

    // Create edit dialog with current values
    // EditPasswordDialog dialog(this, pwd);

    // If confirmed
    // if (dialog.exec() == QDialog::Accepted) {
    //     _db->updatePassword(id, ...);
    //     updateUi();
    // }

    this->updateUi();
}

void MainWindow::onDeletePassword(int row)
{
    PrintLog(std::cout, MAGENTA "Delete Password" RESET " for %d row", row);

    // Confirm
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Password", "Are you sure to delete password", QMessageBox::Yes | QMessageBox::No);

    Password pwd;
    if (!_db->getPassword(row, pwd))
        QMessageBox::warning(this, "Warning", "password not found in the db");

    // Delete and refresh
    if (reply == QMessageBox::Yes)
        _db->deletePassword(row);
    this->updateUi();
}
