#include "signup.h"
#include "qregularexpression.h"
#include "ui_signup.h"
#include "startwindow.h"

signUp::signUp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::signUp)
{
    ui->setupUi(this);

    this->setWindowTitle("Home Management");
    this->setWindowIcon(QIcon(":/Resources/icon.ico"));
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");

    connect(ui->usernameLineEdit, &QLineEdit::returnPressed, this, &signUp::on_signUpButton_clicked);
    connect(ui->passwordLineEdit1, &QLineEdit::returnPressed, this, &signUp::on_signUpButton_clicked);
    connect(ui->passwordLineEdit2, &QLineEdit::returnPressed, this, &signUp::on_signUpButton_clicked);
    connect(ui->emailLineEdit, &QLineEdit::returnPressed, this, &signUp::on_signUpButton_clicked);
}

signUp::~signUp()
{
    delete ui;
}

void signUp::on_logInButton_clicked()
{
    StartWindow *logInWindow = new StartWindow;
    this->close();
    logInWindow->show();
}


void signUp::on_signUpButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password1 = ui->passwordLineEdit1->text();
    QString password2 = ui->passwordLineEdit2->text();
    QString email = ui->emailLineEdit->text();
    float budget = ui->budgetSpinBox->value();

    // Sprawdzenie, czy wszystkie pola są wypełnione
    if (username.isEmpty() || password1.isEmpty() || password2.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Wszystkie pola muszą być wypełnione");
        return;
    }

    // Sprawdzenie, czy oba hasła się zgadzają
    if (password1 != password2) {
        QMessageBox::warning(this, "Błąd", "Podane hasła nie są identyczne");
        return;
    }

    // Sprawdzenie warunków dla hasła
    QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    if (!passwordRegex.match(password1).hasMatch()) {
        QMessageBox::warning(this, "Błąd", "Hasło musi mieć przynajmniej 1 dużą literę, 1 cyfrę i minimum 8 znaków");
        return;
    }

    // Sprawdzenie warunków dla adresu email
    QRegularExpression emailRegex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Błąd", "Nieprawidłowy adres email");
        return;
    }

    db.open();

    // Sprawdzenie, czy użytkownik o podanej nazwie użytkownika lub emailu już istnieje
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM users WHERE username = :username OR email = :email");
    checkQuery.bindValue(":username", username);
    checkQuery.bindValue(":email", email);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Błąd", "Użytkownik o podanej nazwie użytkownika lub emailu już istnieje");
        db.close();
        return;
    }

    // Dodanie nowego użytkownika do bazy danych
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, email, budget) VALUES (:username, :password, :email, :budget)");
    query.bindValue(":username", username);
    query.bindValue(":password", password1);
    query.bindValue(":email", email);
    query.bindValue(":budget", budget);

    if (query.exec()) {
        QMessageBox::information(this, "Sukces", "Rejestracja udana. Możesz się zalogować.");
    } else {
        QMessageBox::critical(this, "Błąd", "Wystąpił problem podczas dodawania użytkownika do bazy danych");
    }

    db.close();
}



