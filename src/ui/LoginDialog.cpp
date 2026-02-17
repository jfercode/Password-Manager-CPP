#include "LoginDialog.hpp"

LoginDialog::LoginDialog(QWidget *parent, AuthenticationManager *auth) : QDialog(parent)
{
    // Window Title
    setWindowTitle("Login Password Manager");

    // Set up dialog ui
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Initialazing UI...");
    setupUi();

    authM = auth;
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

    // Validate both fields filled
    if (user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    // Check for authenticator manager
    if (!authM)
    {
        QMessageBox::warning(this, "Error", "Authentication system not initialized");
        return;
    }

    // Authenticate user with the auth Manager
    if (authM->authenticateUser(user.toStdString(), pass.toStdString()))
    {
        PrintLog(std::cout, GREEN "Login successful for user: %s" RESET, user.toStdString().c_str());
        accept();
    }
    else // Authentication failed
    {
        QMessageBox::warning(this, "Error", "Invalid username or password");
        userEdit->clear();
        passEdit->clear();
        userEdit->setFocus();
    }
}

void LoginDialog::onCancelClicked()
{
    // Confirm before canceling
    int reply = QMessageBox::question(this, "Cancel Registration",
                                      "Are you sure you want to cancel?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        reject(); // User confirmed - close dialog
}
