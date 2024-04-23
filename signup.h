#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class signUp;
}

class signUp : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase db;
    QSqlQuery query;
    explicit signUp(QWidget *parent = nullptr);
    ~signUp();

private slots:

    void on_logInButton_clicked();

    void on_signUpButton_clicked();

private:
    Ui::signUp *ui;
};

#endif // SIGNUP_H
