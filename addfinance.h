#ifndef ADDFINANCE_H
#define ADDFINANCE_H

#include "dashboard.h"
#include "qdatetime.h"
#include <QDialog>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class addFinance;
}

class addFinance : public QDialog
{
    Q_OBJECT

signals:
    void transactionAdded(int userID);

    void transactionUpdated (int userID, int financeID);

public:
    QSqlDatabase db;
    explicit addFinance(QWidget *parent = nullptr, int userID = -1, int pageID = 0, int financeid = -1);
    ~addFinance();

private slots:
    void on_confirmButton_clicked();

    void on_confirmButton_3_clicked();

private:
    Dashboard *dashboard = new Dashboard(nullptr, id);

    Ui::addFinance *ui;

    int financeid;

    int id = -1;

    QString type;

    double amount;

    QString title;

    QDate date;

    QString category;

    void setParametersToEdit(int financeID);
};

#endif // ADDFINANCE_H
