#include "EditPasswordDialog.hpp"

EditPasswordDialog::EditPasswordDialog(QWidget *parent, SQLiteCipherDB *db, int id)
    : QDialog(parent), _db(db), _passwordId(id)
{
    // Window Title
    setWindowTitle("Edit Password Manager");

    // Set up dialog ui
    PrintLog(std::cout, YELLOW "Edit Password Dialog" RESET " - Initialazing UI for %d ID password", _passwordId);
    setupUi();

    // Populate fields with existing password data
    if (id)
    {
        // Obtain all passwords from db
        std::vector<Password> passwords = _db->getAllPasswords();
        // Iterate for each password in the vector
        for (const auto &pwd : passwords)
        {
            if (pwd.id == id)
            {
                webEdit->setText(QString::fromStdString(pwd.website));
                webStr = pwd.website;
                userEdit->setText(QString::fromStdString(pwd.username));
                userStr = pwd.username;
                passEdit->setText(QString::fromStdString(pwd.encrypted_password));
                passStr = pwd.encrypted_password;
                break;
            }
        }
    }

    // Connect signal to slot
    PrintLog(std::cout, YELLOW "Edit Password Dialog" RESET " - Establishing buttons connection...");
    connect(saveBttn, &QPushButton::clicked, this, &EditPasswordDialog::onSaveClicked);
    connect(cancelBttn, &QPushButton::clicked, this, &EditPasswordDialog::onCancelClicked);
}

EditPasswordDialog::~EditPasswordDialog() {}

void EditPasswordDialog::setupUi()
{
    // Widgets creation
    webEdit = new QLineEdit(this);
    userEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);

    saveBttn = new QPushButton("Editar contraseña", this);
    cancelBttn = new QPushButton("Cancelar", this);

    // Widget configuration
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

void EditPasswordDialog::onSaveClicked()
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

    // Validate at least diferent password
    if (pass.toStdString() == passStr)
    {
        QMessageBox::warning(this, "Warning", "Same password detected");
        return;
    }
    // Check for authenticator manager
    if (!_db)
    {
        QMessageBox::warning(this, "Error", "Database not initialized");
        return;
    }
    if (_db->updatePassword(_passwordId,
                            web.toStdString(),
                            user.toStdString(),
                            pass.toStdString()))
    {
        PrintLog(std::cout, GREEN "Password edited for %s" RESET, web.toStdString().c_str());
        QMessageBox::information(this, "Success", "Password edited successfully!");
        accept(); // Cerrar dialog
    }
    else
        QMessageBox::warning(this, "Error", "Failed to edit password");
}

void EditPasswordDialog::onCancelClicked()
{
    // Confirm before canceling
    int reply = QMessageBox::question(this, "Cancel Edition",
                                      "Are you sure you want to cancel?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        reject(); // User confirmed - close dialog
}
