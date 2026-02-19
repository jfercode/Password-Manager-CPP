#include "MainWindow.hpp"

// MainWindow Constructor
MainWindow::MainWindow() : QMainWindow()
{
    // Window Setup
    setWindowTitle("Password Manager - Secure Storage");
    // TODO: Add icon
    // setWindowIcon(QIcon(":/icon route"));

    // Set up Ui
    PrintLog(std::cout, YELLOW "Main Window" RESET " - Initialazing UI...");
    setupUI();

    // Conect bttns to functions here
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
    resize(WIDTH, HEIGHT); // TODO: make widthxheight tests
    setMinimumSize(WIDTH / 2, HEIGHT / 2);

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
    std::string title = "Your Passwords [" + SESSION->getUsername() + "]";
    QLabel *tittleLabel = new QLabel(title.c_str(), this);
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
    passwordTable->verticalHeader()->setDefaultSectionSize(60);
    passwordTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    passwordTable->setHorizontalHeaderLabels({"Website", "Username", "Password", "Actions"});

    // Configurar el tamaño de cada columna
    passwordTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // Website
    passwordTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Username
    passwordTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); // Password
    passwordTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch); // Actions

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

    logoutBttn = new QPushButton("Logout", this);
    logoutBttn->setMinimumWidth(150);

    bttnLayout->addWidget(addBttn);
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
    // Table minimun size
    setMinimumSize(800, 500);

    // Get database from SessionManager
    SQLiteCipherDB *db = SESSION->getDatabase();
    CryptoManager *crypt = SESSION->getCryptoManager();

    if (!db || !crypt)
    {
        QMessageBox::critical(this, "Error", "Some service are not available");
        return;
    }

    // Clean current passwordTable
    if (passwordTable->rowCount() > 0)
        passwordTable->setRowCount(0);

    // Obtain all passwords from db
    std::vector<Password> passwords = db->getPasswordsByUserId(SESSION->getUserId());

    // Iterate for each password in the vector
    for (const auto &pwd : passwords)
    {
        // New row for each iteration
        int row = passwordTable->rowCount();
        passwordTable->insertRow(row);

        // WEB ITEM
        QTableWidgetItem *webItem = new QTableWidgetItem(QString::fromStdString(pwd.website));
        webItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        // WEB USER ITEM
        QTableWidgetItem *userItem = new QTableWidgetItem(QString::fromStdString(pwd.username));
        userItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        // WEB USER PASS ITEM
        QLineEdit *pwdEdit = new QLineEdit(this);

        // Decrypt password before show in the ui
        std::string password_decrypt = crypt->decryptPassword(
            pwd.encrypted_password,
            pwd.iv, SESSION->getMasterPassword(),
            SESSION->getUserSalt());

        pwdEdit->setText(QString::fromStdString(password_decrypt));
        pwdEdit->setEchoMode(QLineEdit::Password); // ← Show "*"
        pwdEdit->setReadOnly(true);
        pwdEdit->setProperty("passwordId", pwd.id); // save ID for later
        pwdEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        // WEB USER PASS ACTION ITEM
        QWidget *actionWidget = new QWidget(this);
        QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(5, 5, 5, 5);
        actionLayout->setSpacing(8);
        actionLayout->setAlignment(Qt::AlignCenter);

        // WEB USER PASS ACTION BUTTONS
        // Crete three buttons for password in db: view, edit and delete
        QPushButton *viewBtn = new QPushButton("👁", this);
        viewBtn->setMaximumWidth(38);
        viewBtn->setMaximumHeight(38);
        QPushButton *editBtn = new QPushButton("✏️", this);
        editBtn->setMaximumWidth(38);
        editBtn->setMaximumHeight(38);
        QPushButton *deleteBtn = new QPushButton("🗑️", this);
        deleteBtn->setMaximumWidth(38);
        deleteBtn->setMaximumHeight(38);

        // Connect butons with functions
        connect(viewBtn, &QPushButton::clicked, this, [this, pwd]()
                { this->onViewPassword(pwd.id); });
        connect(editBtn, &QPushButton::clicked, this, [this, pwd]()
                { this->onEditPassword(pwd.id); });
        connect(deleteBtn, &QPushButton::clicked, this, [this, pwd]()
                { this->onDeletePassword(pwd.id); });
        // Add buttons to action layout
        actionLayout->addWidget(viewBtn);
        actionLayout->addWidget(editBtn);
        actionLayout->addWidget(deleteBtn);

        // Put widgets into passwordTable
        passwordTable->setItem(row, 0, webItem);
        passwordTable->setItem(row, 1, userItem);
        passwordTable->setCellWidget(row, 2, pwdEdit);
        passwordTable->setCellWidget(row, 3, actionWidget);
    }
}

// Buttons handle
void MainWindow::onClickAddPssBttn()
{
    PrintLog(std::cout, MAGENTA "Add Password Button" RESET " - Adding a new password...");

    // Create add password dialog
    AddPasswordDialog dialog(this);

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
    {
        SESSION->clearSession();
        this->close();
    }
}

void MainWindow::onViewPassword(int id)
{
    PrintLog(std::cout, MAGENTA "View Password" RESET " for ID %d", id);

    // Find the file with the id
    for (int row = 0; row < passwordTable->rowCount(); row++)
    {
        QLineEdit *pwdEdit = qobject_cast<QLineEdit *>(passwordTable->cellWidget(row, 2));
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

    // Get database from SessionManager
    SQLiteCipherDB *db = SESSION->getDatabase();
    if (!db)
    {
        QMessageBox::critical(this, "Error", "Database service not available");
        return;
    }

    // Obtain db password
    Password pwd;
    if (!db->getPassword(id, pwd))
    {
        QMessageBox::warning(this, "Error", "Password not found");
        return;
    }
    EditPasswordDialog dial(this, id);
    if (dial.exec() == QDialog::Accepted)
        updateUi();
}

void MainWindow::onDeletePassword(int id)
{
    PrintLog(std::cout, MAGENTA "Delete Password" RESET " with %d ID", id);

    // Get database from SessionManager
    SQLiteCipherDB *db = SESSION->getDatabase();
    if (!db)
    {
        QMessageBox::critical(this, "Error", "Database service not available");
        return;
    }

    // Confirm
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Password", "Are you sure to delete password", QMessageBox::Yes | QMessageBox::No);

    Password pwd;
    if (!db->getPassword(id, pwd))
        QMessageBox::warning(this, "Warning", "password not found in the db");

    // Delete and refresh
    if (reply == QMessageBox::Yes)
        db->deletePassword(id);
    this->updateUi();
}
