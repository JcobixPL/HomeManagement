#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class account;
}

class account : public QDialog
{
    Q_OBJECT

signals:
    void updateAccount(int userID);

public:
    QSqlDatabase db;
    explicit account(QWidget *parent = nullptr, int id = -1);
    ~account();

private slots:
    void on_confrimButton_clicked();

private:
    Ui::account *ui;
    int userid;
};

#endif // ACCOUNT_H
