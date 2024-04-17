#ifndef TASK_H
#define TASK_H

#include "dashboard.h"
#include "qdatetime.h"
#include <QDialog>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class task;
}

class task : public QDialog
{
    Q_OBJECT

signals:
    void taskAdded(int userID);

    void taskUpdated (int userID, int taskID);

public:
    QSqlDatabase db;
    explicit task(QWidget *parent = nullptr, int userID = -1, int pageID = 0, int taskid = -1);
    ~task();

private slots:
    void on_confirmTaskButton_clicked();

    void on_confirmTaskButton_2_clicked();

private:
    Ui::task *ui;

    int id = -1;

    int taskid;

    QString title;

    int importance;

    QDate date;

    QString description;

    QString done;

    void setParametersToEdit(int taskID);
};

#endif // TASK_H
