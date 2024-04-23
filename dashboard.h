#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QSqlRecord>
#include "account.h"
#include "task.h"

namespace Ui {
class Dashboard;
}

class Dashboard : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase db;
    explicit Dashboard(QWidget *parent = nullptr, int userID = -1);
    ~Dashboard();
    void updateTable();
    void updateAllTasks();
    void updateDayTasks();
    void updateTodayTasks();
    void updateFinances(int id);
    void updateBudget(int id);
    void updateEditedFinances(int id, int financeid);
    void updateRemoveFinances(int id);
    void createPieChart();
    void createLineChart();
    void updateLineChart();
    void updatePieChart();
    QStringList getLast6Months();
    QVector<double> getTransactionSumsByType(const QString& transactionType, int userId);
private slots:
    void updateTime();

    void openQuitDialog();

    void on_quitButton_clicked();

    void on_quitButton_2_clicked();

    void on_quitButton_3_clicked();

    void on_quitButton_4_clicked();

    void on_financesButton_clicked();

    void on_tasksButton_clicked();

    void on_accountButton_clicked();

    void on_dashboardButton_2_clicked();

    void on_tasksButton_2_clicked();

    void on_accountButton_2_clicked();

    void on_dashboardButton_3_clicked();

    void on_financesButton_3_clicked();

    void on_accountButton_3_clicked();

    void on_dashboardButton_4_clicked();

    void on_financesButton_4_clicked();

    void on_tasksButton_4_clicked();

    void on_deleteAccountButton_clicked();

    void on_editAccountButton_clicked();

    void on_addFinanceButton_clicked();

    void on_dayBox_currentIndexChanged(int index);

    void on_passwordCheckBox_clicked();

    void on_editFinanceButton_clicked();

    void on_removeFinanceButton_clicked();

    void on_addTaskButton_clicked();

    void on_editTaskButton_clicked();

    void on_removeTaskButton_clicked();

    void on_makeAsDoneButton_clicked();

    void on_makeAsUndoneButton_clicked();

private:
    Ui::Dashboard *ui;

    int id = -1;

    int userid;

    int financeid;

    float budget;

    float balance;

    float amount;

    float oldAmount;

    QString oldType;

    QString formattedBudget;

    QString formattedIncomes;

    QString formattedExpenses;

    QString formattedBalance;

    QString type;

    float incomes;

    float expenses;

    QString username;

    QString email;

    QString password;

    QTimer *timer;

    void updateTableView(QSqlTableModel *model, QTableView *tableView, QLabel *titleLabel, QLabel *dateLabel, QLabel *importanceLabel, QLabel *doneLabel, QLabel *descriptionLabel);

};

#endif // DASHBOARD_H
