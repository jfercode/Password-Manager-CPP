#include "NewUserDialog.hpp"

NewUserDialog::NewUserDialog(QWidget *parent, AuthenticationManager *auth) : QDialog(parent)
{
    // Window Title
    setWindowTitle("New User Login Password Manager");

    // Set up dialog ui
    PrintLog(std::cout, YELLOW "New User Login Dialog" RESET " - Initialazing UI...");
    setupUi();

    authM = auth;
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

    // Minimal login // TODO: Change this in a future
    // if (user == "admin" && pass == "1234")

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

    // TODO check for password security in a future

    // check if username already exists
    if (authM && authM->authenticateUser(user.toStdString(), pass.toStdString()))
    {
        QMessageBox::warning(this, "Error", "User already exists");
        userEdit->clear();
        passEdit->clear();
        verifyEdit->clear();
        return;
    }
    
    // All validation completed - register user
    if (authM && authM->registerNewUser(user.toStdString(), pass.toStdString(), true))
    {
        QMessageBox::information(this, "Success", "User registered successfully");
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