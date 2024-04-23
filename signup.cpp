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

    if (username.isEmpty() || password1.isEmpty() || password2.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Warning", "All fields need to be filled in");
        return;
    }

    if (password1 != password2) {
        QMessageBox::warning(this, "Warning", "Both given passwords don't match each other");
        return;
    }

    QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    if (!passwordRegex.match(password1).hasMatch()) {
        QMessageBox::warning(this, "Warning", "Password must have at least 8 characters, lowercase and uppercase letters and numbers");
        return;
    }

    QRegularExpression emailRegex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Warning", "Incorrect email address");
        return;
    }
    db.open();

    // Sprawdzenie, czy użytkownik o podanej nazwie użytkownika lub emailu już istnieje
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM users WHERE username = :username OR email = :email");
    checkQuery.bindValue(":username", username);
    checkQuery.bindValue(":email", email);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Warning", "User with given username or email already exists in database");
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



