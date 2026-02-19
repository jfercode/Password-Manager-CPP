#include "AddPasswordDialog.hpp"

AddPasswordDialog::AddPasswordDialog(QWidget *parent) : QDialog(parent)
{
    // Window Title
    setWindowTitle("Add Password Manager");

    // Set up dialog ui
    PrintLog(std::cout, YELLOW "Add Password Dialog" RESET " - Initialazing UI...");
    setupUi();

    // Connect signal to slot
    PrintLog(std::cout, YELLOW "Add Password Dialog" RESET " - Establishing buttons connection...");
    connect(saveBttn, &QPushButton::clicked, this, &AddPasswordDialog::onSaveClicked);
    connect(cancelBttn, &QPushButton::clicked, this, &AddPasswordDialog::onCancelClicked);
}

AddPasswordDialog::~AddPasswordDialog() {}

void AddPasswordDialog::setupUi()
{
    // Widgets creation
    webEdit = new QLineEdit(this);
    userEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);

    saveBttn = new QPushButton("Guardar contraseña", this);
    cancelBttn = new QPushButton("Cancelar", this);

    // Widget configuration
    webEdit->setPlaceholderText("Website (e.g., Gmail, Facebook...)");
    userEdit->setPlaceholderText("Usuario");
    passEdit->setPlaceholderText("Contraseña");

    passEdit->setEchoMode(QLineEdit::Password); // hide password

    // Vertical Principal Layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Website", this));
    layout->addWidget(webEdit);
    layout->addWidget(new QLabel("Usuario", this));
    layout->addWidget(userEdit);
    layout->addWidget(new QLabel("Contraseña", this));
    layout->addWidget(passEdit);

    // Buttons  Horizontal Layout
    QHBoxLayout *bttnLayout = new QHBoxLayout();
    bttnLayout->addWidget(saveBttn);
    bttnLayout->addWidget(cancelBttn);

    // Add button layout into the principal layout
    layout->addLayout(bttnLayout);
}

void AddPasswordDialog::onSaveClicked()
{
    QString web = webEdit->text().trimmed();
    QString user = userEdit->text().trimmed();
    QString pass = passEdit->text();

    // Validate both fields filled
    if (user.isEmpty() || pass.isEmpty() || web.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    // Get services from SessionManager
    SQLiteCipherDB *db = SESSION->getDatabase();
    CryptoManager *crypto = SESSION->getCryptoManager();

    // Check for database and crypto
    if (!db || !crypto)
    {
        QMessageBox::warning(this, "Error", "Database or Crypto service not initialized");
        return;
    }

    // Get master password and salt from session
    std::string masterPassword = SESSION->getMasterPassword();
    std::string userSalt = SESSION->getUserSalt();

    auto [ciphertext, iv] = crypto->encryptPassword(
        pass.toStdString(),
        masterPassword,
        userSalt
    );
    
    // Add the password to the db
    if (db->addPassword(SESSION->getUserId(), web.toStdString(), user.toStdString(), ciphertext, iv)) 
    {
        PrintLog(std::cout, GREEN "Password saved for %s" RESET, web.toStdString().c_str());
        QMessageBox::information(this, "Success", "Password saved successfully!");
        accept(); // Cerrar dialog
    }
    else
        QMessageBox::critical(this, "Error", "Failed to save password");
}

void AddPasswordDialog::onCancelClicked()
{
    // Confirm before canceling
    int reply = QMessageBox::question(this, "Cancel Registration",
                                      "Are you sure you want to cancel?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        reject(); // User confirmed - close dialog
}
