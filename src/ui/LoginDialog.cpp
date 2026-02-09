#include "../../include/LoginDialog.hpp"

// 
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    // Window Title
    setWindowTitle("Login Password Manager");

    // Set up dialog ui
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Initialazing UI...");    
    setupUi();
    
    // Connect signal to slot
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Establishing buttons connection...");    
    connect(loginBttn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(cancelBttn, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
}

LoginDialog::~LoginDialog() {}

void LoginDialog::setupUi()
{
    // Widgets creation
    userEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);
    loginBttn = new QPushButton("Login", this);
    cancelBttn = new QPushButton("Cancelar", this);

    // Widget configuration
    userEdit->setPlaceholderText("Usuario");
    passEdit->setPlaceholderText("Contraseña");
    passEdit->setEchoMode(QLineEdit::Password);

    // Vertical Principal Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Usuario", this));
    layout->addWidget(userEdit);
    layout->addWidget(new QLabel("Contraseña", this));
    layout->addWidget(passEdit);

    // Buttons  Horizontal Layout
    QHBoxLayout *bttnLayout = new QHBoxLayout();
    bttnLayout->addWidget(loginBttn);
    bttnLayout->addWidget(cancelBttn);

    // Add button layout into the principal layout
    layout->addLayout(bttnLayout);
}

void LoginDialog::onLoginClicked()
{
    QString user = userEdit->text();
    QString pass = passEdit->text();

    // Minimal login // TODO: Change this in a future
    if (user == "admin" && pass == "1234")
        accept();
    else
        QMessageBox::warning(this, "Error", "Credeniales incorrectas");
}

void LoginDialog::onCancelClicked()
{
    // Reject dialog (login) ->  QDialog::Rejected
    reject();
}
