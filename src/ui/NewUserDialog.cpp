#include "NewUserDialog.hpp"

NewUserDialog::NewUserDialog(QWidget *parent) : QDialog(parent)
{
    // Window Title
    setWindowTitle("New User Login Password Manager");

    // Set up dialog ui
    PrintLog(std::cout, YELLOW "New User Login Dialog" RESET " - Initialazing UI...");
    setupUi();

    // Connect signal to slot
    PrintLog(std::cout, YELLOW "New User Login Dialog" RESET " - Establishing buttons connection...");
    connect(loginBttn, &QPushButton::clicked, this, &NewUserDialog::onLoginClicked);
    connect(cancelBttn, &QPushButton::clicked, this, &NewUserDialog::onCancelClicked);
}

NewUserDialog::~NewUserDialog() {}

void NewUserDialog::setupUi()
{
    // Widgets creation
    userEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);
    verifyEdit = new QLineEdit(this);
    loginBttn = new QPushButton("Login", this);
    cancelBttn = new QPushButton("Cancelar", this);

    // Widget configuration
    userEdit->setPlaceholderText("Usuario");
    passEdit->setPlaceholderText("Contraseña");
    verifyEdit->setPlaceholderText("Verificar Contraseña");
    passEdit->setEchoMode(QLineEdit::Password);
    verifyEdit->setEchoMode(QLineEdit::Password);

    // Vertical Principal Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Usuario", this));
    layout->addWidget(userEdit);
    layout->addWidget(new QLabel("Contraseña", this));
    layout->addWidget(passEdit);
    layout->addWidget(new QLabel("Verificar Contraseña", this));
    layout->addWidget(verifyEdit);

    // Buttons  Horizontal Layout
    QHBoxLayout *bttnLayout = new QHBoxLayout();
    bttnLayout->addWidget(loginBttn);
    bttnLayout->addWidget(cancelBttn);

    // Add button layout into the principal layout
    layout->addLayout(bttnLayout);
}

void NewUserDialog::onLoginClicked()
{
    QString user = userEdit->text();
    QString pass = passEdit->text();
    QString passVer = verifyEdit->text();

    // verify variables
    if (user.isEmpty() || pass.isEmpty() || passVer.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    // pass == passVer validation
    if (pass != passVer)
    {
        QMessageBox::warning(this, "Error", "Passwords not match");
        passEdit->clear();
        verifyEdit->clear();
        return;
    }

    // Check for auth manager
    if (!SESSION->getAuthenticationManager())
    {
        QMessageBox::warning(this, "Error", "Authentication system not initialized");
        return;
    }

    // check if username already exists
    if (SESSION->getAuthenticationManager()->authenticateUser(user.toStdString(), pass.toStdString()))
    {
        QMessageBox::warning(this, "Error", "User already exists");
        userEdit->clear();
        passEdit->clear();
        verifyEdit->clear();
        return;
    }

    // // Check for password security
    // if (!SESSION->getCryptoManager()->validatePassword(pass.toStdString()))
    // {
    //     QMessageBox::warning(this, "Error", "Password not secure");
    //     passEdit->clear();
    //     verifyEdit->clear();
    //     return;
    // }

    // All validation completed - register user
    if (SESSION->getAuthenticationManager()->registerNewUser(user.toStdString(), pass.toStdString(), true))
    {
        QMessageBox::information(this, "Success", "User registered successfully");

        // Initialize session with new user
        std::string salt;
        std::string hash;
        if (SESSION->getDatabase() && SESSION->getDatabase()->getUserHash(user.toStdString(), hash, salt))
        {
            SESSION->setMasterPassword(pass.toStdString());
            SESSION->setUsername(user.toStdString());
            SESSION->setUserSalt(salt);
            SESSION->setAuthenticated(true);

            PrintLog(std::cout, CYAN "SessionManager" GREEN " - Session initialized for new user" RESET);
        }

        accept();
    }
    else
        QMessageBox::warning(this, "Error", "Credeniales incorrectas");
}

void NewUserDialog::onCancelClicked()
{
    // Confirm before canceling
    int reply = QMessageBox::question(this, "Cancel Registration",
                                      "Are you sure you want to cancel?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        reject(); // User confirmed - close dialog
}