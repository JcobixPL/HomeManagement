#include "account.h"
#include "ui_account.h"

account::account(QWidget *parent, int id)
    : QDialog(parent)
    , ui(new Ui::account)
{
    userid = id;
    ui->setupUi(this);
    this->setWindowTitle("Home Management");
    this->setWindowIcon(QIcon(":/Resources/icon.ico"));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT username, email, password, budget FROM users WHERE userid = :userid");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        ui->usernameLineEdit->setText(query.value(0).toString());
        ui->emailLineEdit->setText(query.value(1).toString());
        ui->budgetSpinBox->setValue(query.value(3).toFloat());
        ui->passwordLineEdit1->setText(query.value(2).toString());
        ui->passwordLineEdit2->setText(query.value(2).toString());
    }
}

account::~account()
{
    delete ui;
}

void account::on_confrimButton_clicked()
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
    checkQuery.prepare("SELECT * FROM users WHERE userid != :userid AND (username = :username OR email = :email");
    checkQuery.bindValue(":username", username);
    checkQuery.bindValue(":email", email);
    checkQuery.bindValue(":userid", userid);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Błąd", "Użytkownik o podanej nazwie użytkownika lub emailu już istnieje");
        db.close();
        return;
    }

    QSqlQuery query;
    qDebug() << userid;
    qDebug() << password1;
    query.prepare("UPDATE users SET username = :username, email = :email, budget = :budget, password = :password WHERE userid = :userid");
    query.bindValue(":username", username);
    query.bindValue(":password", password1);
    query.bindValue(":email", email);
    query.bindValue(":budget", budget);
    query.bindValue(":userid", userid);
    query.exec();
    db.close();
    emit updateAccount(userid);
    this->close();
}

