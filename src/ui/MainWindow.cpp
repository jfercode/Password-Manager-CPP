#include "../../include/MainWindow.hpp"

// MainWindow Constructor
MainWindow::MainWindow(void)
{
    // Window Setup
    setWindowTitle("Password Manager - Secure Storage");
    // TODO: Add icon
    //setWindowIcon(QIcon(":/icon route"));

    // Set up Ui
    PrintLog(std::cout, YELLOW "Main Window" RESET " - Initialazing UI...");    
    setupUI();

    // TODO Conect bttns to functions here
    PrintLog(std::cout, YELLOW "Main Window" RESET " - Establishing buttons connection...");
    connect(addBttn, &QPushButton::clicked, this, &MainWindow::onClickAddPssBttn);
    connect(deleteBttn, &QPushButton::clicked, this, &MainWindow::onClickDeleteBttn);
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
    setMinimumSize(WIDTH/2, HEIGTH/2);

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
    QLineEdit *searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Search passwords...");
    searchBox->setMaximumWidth(250);

    headerLayout->addWidget(tittleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(searchBox);

    mainLayout->addWidget(tittleLabel);

    // ============ TABLE SECTION ============ //
    QTableWidget *passwordTable = new QTableWidget(this);
    passwordTable->setColumnCount(4);
    passwordTable->setHorizontalHeaderLabels({"Website", "Username","Password", "Actions"});
    passwordTable->horizontalHeader()->setStretchLastSection(false);
    passwordTable->setColumnWidth(0, 200);
    passwordTable->setColumnWidth(1, 200);
    passwordTable->setColumnWidth(2, 150);
    passwordTable->setColumnWidth(3, 150);
    passwordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    passwordTable->setSelectionMode(QAbstractItemView::SingleSelection);
    passwordTable->setAlternatingRowColors(true);
    
    // Add sample data (TODO: Remove this later - just for demonstration) 
    passwordTable->insertRow(0);
    passwordTable->setItem(0, 0, new QTableWidgetItem("Gmail"));
    passwordTable->setItem(0, 1, new QTableWidgetItem("john@example.com"));
    passwordTable->setItem(0, 2, new QTableWidgetItem("••••••••"));
    
   // Action Layout - CREATE A CONTAINER WIDGET
    QWidget *actionWidget = new QWidget(this);
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(2, 2, 2, 2);
    actionLayout->setSpacing(5);
    
    // Create buttons
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
    
    // SET THE WIDGET IN THE TABLE CELL
    passwordTable->setCellWidget(0, 3, actionWidget);
    
    // Connect buttons to slots
    // connect(viewBtn, &QPushButton::clicked, this, &MainWindow::onClickViewBtn);
    // connect(editBtn, &QPushButton::clicked, this, &MainWindow::onClickEditBtn);
    // connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onClickDeleteBtn);
    
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
}

// Buttons handle
void    MainWindow::onClickAddPssBttn()
{
    PrintLog(std::cout,  MAGENTA "Add Password Button" RESET " - Adding a new password...");
    // TODO: Implementar Añadir contenido
}

void    MainWindow::onClickLogoutBttn() 
{
    PrintLog(std::cout,  MAGENTA "Logout Button" RESET " - Loging out...");
    // TODO: Implementar logout

}
void    MainWindow::onClickDeleteBttn() 
{
    PrintLog(std::cout,  MAGENTA "Delete Password Button" RESET " - Handling delete password...");
    // TODO: Implementar Delete

}
