#include "startwindow.h"
#include "ui_startwindow.h"
#include "signup.h"
#include "dashboard.h"

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Home Management");
    this->setWindowIcon(QIcon(":/Resources/icon.ico"));
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");

    connect(ui->usernameLineEdit, &QLineEdit::returnPressed, this, &StartWindow::on_logInButton_clicked);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &StartWindow::on_logInButton_clicked);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_logInButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    db.open();
    QSqlQuery query;
    query.prepare("SELECT userid FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (query.exec() && query.next()) {
        int userID = query.value(0).toInt();
        Dashboard *dashboardWindow = new Dashboard(nullptr, userID);
        this->hide();
        dashboardWindow->show();
    }
    else {
        QMessageBox::information(this, "Logowanie", "Błędna nazwa użytkownika lub hasło!");
    }
    db.close();
}


void StartWindow::on_signUpButton_clicked()
{
    signUp *signUpWindow = new signUp;
    this->hide();
    signUpWindow->show();
}

