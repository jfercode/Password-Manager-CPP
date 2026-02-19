#include "LoginDialog.hpp"
#include <cctype>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    // Window Title
    setWindowTitle("Password Manager - Authentication");
    setMinimumSize(400, 350);

    // Set up dialog ui
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Initializing UI...");
    setupUi();

    // Connect signals to slots
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Establishing buttons connection...");
    connect(loginBttn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(registerBttn, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
    connect(cancelBttn, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
    connect(showPassCheckbox, &QCheckBox::toggled, this, &LoginDialog::onShowPasswordToggled);
    connect(regPassEdit, &QLineEdit::textChanged, this, &LoginDialog::onPasswordChanged);
}

LoginDialog::~LoginDialog() {}

void LoginDialog::setActiveTab(int tabIndex)
{
    if (tabWidget)
    {
        tabWidget->setCurrentIndex(tabIndex);
    }
}

void LoginDialog::setupUi()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // Title
    QLabel *titleLabel = new QLabel("Password Manager", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Tab Widget
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Setup tabs
    setupLoginTab();
    setupRegisterTab();

    // Buttons layout (shared)
    QHBoxLayout *bttnLayout = new QHBoxLayout();
    bttnLayout->setSpacing(8);
    bttnLayout->addStretch();

    cancelBttn = new QPushButton("Cancelar", this);
    cancelBttn->setMinimumWidth(100);
    bttnLayout->addWidget(cancelBttn);

    mainLayout->addLayout(bttnLayout);
    setLayout(mainLayout);
}

void LoginDialog::setupLoginTab()
{
    QWidget *loginTab = new QWidget(this);
    QVBoxLayout *loginLayout = new QVBoxLayout(loginTab);
    loginLayout->setContentsMargins(20, 20, 20, 20);
    loginLayout->setSpacing(10);

    // Username field
    loginLayout->addWidget(new QLabel("Usuario:", this));
    loginUserEdit = new QLineEdit(this);
    loginUserEdit->setPlaceholderText("Ingresa tu usuario");
    loginLayout->addWidget(loginUserEdit);

    // Password field
    loginLayout->addWidget(new QLabel("Contraseña:", this));
    loginPassEdit = new QLineEdit(this);
    loginPassEdit->setPlaceholderText("Ingresa tu contraseña");
    loginPassEdit->setEchoMode(QLineEdit::Password);
    loginLayout->addWidget(loginPassEdit);

    // Show password checkbox
    showPassCheckbox = new QCheckBox("Mostrar contraseña", this);
    loginLayout->addWidget(showPassCheckbox);

    loginLayout->addStretch();

    // Login button
    loginBttn = new QPushButton("Entrar", this);
    loginBttn->setMinimumHeight(35);
    loginLayout->addWidget(loginBttn);

    tabWidget->addTab(loginTab, "Iniciar Sesión");
}

void LoginDialog::setupRegisterTab()
{
    QWidget *registerTab = new QWidget(this);
    QVBoxLayout *regLayout = new QVBoxLayout(registerTab);
    regLayout->setContentsMargins(20, 20, 20, 20);
    regLayout->setSpacing(10);

    // Username field
    regLayout->addWidget(new QLabel("Usuario:", this));
    regUserEdit = new QLineEdit(this);
    regUserEdit->setPlaceholderText("Elige un usuario");
    regLayout->addWidget(regUserEdit);

    // Password field
    regLayout->addWidget(new QLabel("Contraseña:", this));
    regPassEdit = new QLineEdit(this);
    regPassEdit->setPlaceholderText("Crea una contraseña fuerte");
    regPassEdit->setEchoMode(QLineEdit::Password);
    regLayout->addWidget(regPassEdit);

    // Password strength indicator
    regLayout->addWidget(new QLabel("Fuerza:", this));
    passwordStrengthBar = new QProgressBar(this);
    passwordStrengthBar->setMaximum(100);
    passwordStrengthBar->setValue(0);
    regLayout->addWidget(passwordStrengthBar);

    strengthLabel = new QLabel("Muy débil", this);
    strengthLabel->setStyleSheet("color: red; font-weight: bold;");
    regLayout->addWidget(strengthLabel);

    // Confirm password field
    regLayout->addWidget(new QLabel("Confirmar Contraseña:", this));
    regConfirmPassEdit = new QLineEdit(this);
    regConfirmPassEdit->setPlaceholderText("Confirma tu contraseña");
    regConfirmPassEdit->setEchoMode(QLineEdit::Password);
    regLayout->addWidget(regConfirmPassEdit);

    regLayout->addStretch();

    // Register button
    registerBttn = new QPushButton("Registrarse", this);
    registerBttn->setMinimumHeight(35);
    regLayout->addWidget(registerBttn);

    tabWidget->addTab(registerTab, "Registrarse");
}

int LoginDialog::calculatePasswordStrength(const std::string &password)
{
    int strength = 0;

    // Length check
    if (password.length() >= 8) strength += 25;
    if (password.length() >= 12) strength += 10;

    // Contains lowercase
    bool hasLower = false;
    for (char c : password) {
        if (islower(c)) {
            hasLower = true;
            break;
        }
    }
    if (hasLower) strength += 15;

    // Contains uppercase
    bool hasUpper = false;
    for (char c : password) {
        if (isupper(c)) {
            hasUpper = true;
            break;
        }
    }
    if (hasUpper) strength += 15;

    // Contains numbers
    bool hasDigit = false;
    for (char c : password) {
        if (isdigit(c)) {
            hasDigit = true;
            break;
        }
    }
    if (hasDigit) strength += 15;

    // Contains special characters
    bool hasSpecial = false;
    const char *special = "!@#$%^&*()_+-=[]{}|;:',.<>?/~`\\";
    for (char c : password) {
        if (strchr(special, c)) {
            hasSpecial = true;
            break;
        }
    }
    if (hasSpecial) strength += 20;

    return std::min(strength, 100);
}

bool LoginDialog::validatePassword(const std::string &password)
{
    // Minimum 8 characters
    if (password.length() < 8) return false;

    // Must have at least one uppercase, one lowercase, one digit
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }

    return hasUpper && hasLower && hasDigit;
}

void LoginDialog::onLoginClicked()
{
    QString user = loginUserEdit->text();
    QString pass = loginPassEdit->text();

    // Validate both fields filled
    if (user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor rellena todos los campos");
        return;
    }

    // Get services from SessionManager
    AuthenticationManager *authM = SESSION->getAuthenticationManager();
    SQLiteCipherDB *db = SESSION->getDatabase();

    // Check for authenticator manager
    if (!authM || !db)
    {
        QMessageBox::warning(this, "Error", "Sistema de autenticación no inicializado");
        return;
    }

    // Authenticate user with the auth Manager
    if (authM->authenticateUser(user.toStdString(), pass.toStdString()))
    {
        PrintLog(std::cout, GREEN "Login successful for user: %s" RESET, user.toStdString().c_str());
        
        std::string salt;
        std::string hash;
        if (db->getUserHash(user.toStdString(), hash, salt))
        {
            int user_id = db->getUserIdByUsername(user.toStdString());
            SESSION->setUserId(user_id);
            SESSION->setMasterPassword(pass.toStdString());
            SESSION->setUsername(user.toStdString());
            SESSION->setUserSalt(salt);
            SESSION->setAuthenticated(true);

            PrintLog(std::cout, CYAN "SessionManager" GREEN " - Session initialized for user ID: %d" RESET, user_id);
        }
        accept();
    }
    else // Authentication failed
    {
        QMessageBox::warning(this, "Error", "Usuario o contraseña inválidos");
        loginUserEdit->clear();
        loginPassEdit->clear();
        loginUserEdit->setFocus();
    }
}

void LoginDialog::onRegisterClicked()
{
    QString user = regUserEdit->text();
    QString pass = regPassEdit->text();
    QString confirmPass = regConfirmPassEdit->text();

    // Validate all fields filled
    if (user.isEmpty() || pass.isEmpty() || confirmPass.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor rellena todos los campos");
        return;
    }

    // Validate username doesn't exist
    SQLiteCipherDB *db = SESSION->getDatabase();
    if (db->userExists(user.toStdString()))
    {
        QMessageBox::warning(this, "Error", "El usuario ya existe");
        regUserEdit->clear();
        regUserEdit->setFocus();
        return;
    }

    // Validate passwords match
    if (pass != confirmPass)
    {
        QMessageBox::warning(this, "Error", "Las contraseñas no coinciden");
        regConfirmPassEdit->clear();
        regConfirmPassEdit->setFocus();
        return;
    }

    // Validate password strength
    if (!validatePassword(pass.toStdString()))
    {
        QMessageBox::warning(this, "Error", 
            "La contraseña debe tener:\n"
            "- Al menos 8 caracteres\n"
            "- Mayúsculas\n"
            "- Minúsculas\n"
            "- Números");
        regPassEdit->clear();
        regConfirmPassEdit->clear();
        regPassEdit->setFocus();
        return;
    }

    // Get services
    AuthenticationManager *authM = SESSION->getAuthenticationManager();
    if (!authM)
    {
        QMessageBox::critical(this, "Error", "Sistema de autenticación no inicializado");
        return;
    }

    // Register user
    if (authM->registerNewUser(user.toStdString(), pass.toStdString(), true))
    {
        PrintLog(std::cout, GREEN "New user registered: %s" RESET, user.toStdString().c_str());
        
        std::string salt;
        std::string hash;
        if (db->getUserHash(user.toStdString(), hash, salt))
        {
            int user_id = db->getUserIdByUsername(user.toStdString());
            SESSION->setUserId(user_id);
            SESSION->setMasterPassword(pass.toStdString());
            SESSION->setUsername(user.toStdString());
            SESSION->setUserSalt(salt);
            SESSION->setAuthenticated(true);

            PrintLog(std::cout, CYAN "SessionManager" GREEN " - Session initialized for new user ID: %d" RESET, user_id);
            QMessageBox::information(this, "Éxito", "¡Usuario registrado correctamente!");
        }
        accept();
    }
    else
    {
        QMessageBox::critical(this, "Error", "No se pudo registrar el usuario");
    }
}

void LoginDialog::onCancelClicked()
{
    // Confirm before canceling
    int reply = QMessageBox::question(this, "Cancelar",
                                      "¿Seguro que quieres salir?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        reject();
}

void LoginDialog::onShowPasswordToggled(bool checked)
{
    if (checked)
    {
        loginPassEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        loginPassEdit->setEchoMode(QLineEdit::Password);
    }
}

void LoginDialog::onPasswordChanged(const QString &pass)
{
    int strength = calculatePasswordStrength(pass.toStdString());
    passwordStrengthBar->setValue(strength);

    if (strength < 25)
    {
        strengthLabel->setText("Muy débil");
        strengthLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    else if (strength < 50)
    {
        strengthLabel->setText("Débil");
        strengthLabel->setStyleSheet("color: orange; font-weight: bold;");
    }
    else if (strength < 75)
    {
        strengthLabel->setText("Medio");
        strengthLabel->setStyleSheet("color: #FFB800; font-weight: bold;");
    }
    else
    {
        strengthLabel->setText("Fuerte");
        strengthLabel->setStyleSheet("color: green; font-weight: bold;");
    }
}
