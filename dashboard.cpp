#include "dashboard.h"
#include "startwindow.h"
#include "ui_dashboard.h"
#include <QTimer>
#include "quitdialog.h"
#include <QDateTime>
#include "addfinance.h"

Dashboard::Dashboard(QWidget *parent, int userID)
    : QDialog(parent)
    , ui(new Ui::Dashboard)
{
    id = userID;
    ui->setupUi(this);
    this->setWindowTitle("Home Management");
    this->setWindowIcon(QIcon(":/Resources/icon.ico"));
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dashboard::updateTime);
    timer->start(1000);
    updateTime();


    ui->stackedWidget->setCurrentIndex(0);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT username, email, password, budget FROM users WHERE userid = :userid");
    query.bindValue(":userid", userID);
    if (query.exec() && query.next()) {
        username = query.value(0).toString();
        email = query.value(1).toString();
        password = query.value(2).toString();
        budget = query.value(3).toFloat();
        formattedBudget = QString::number(budget, 'f', 2);
    }

    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Income' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", userID);
    if (query.exec() && query.next()) {
        incomes = query.value(0).toFloat();
        formattedIncomes = QString::number(incomes, 'f', 2);
    }

    // Dla wydatków (Expense)
    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Expense' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", userID);
    if (query.exec() && query.next()) {
        expenses = query.value(0).toFloat();
        formattedExpenses = QString::number(expenses, 'f', 2);
    }

    ui->emailLabel->setText(email);
    ui->emailLabel_2->setText(email);
    ui->emailLabel_3->setText(email);
    ui->emailLabel_4->setText(email);

    ui->welcomeLabel->setText("Welcome " + username + "!");
    ui->welcomeLabel_3->setText("Welcome " + username + "!");
    ui->welcomeLabel_2->setText("Welcome " + username + "!");
    ui->welcomeLabel_4->setText("Welcome " + username + "!");

    ui->usernameLabel_2->setText(username);
    ui->emailLabel_6->setText(email);
    ui->passwordLabel_2->setText("**********");

    balance = incomes - expenses;
    formattedBalance = QString::number(balance, 'f', 2);

    ui->budgetLabel->setText(formattedBudget + " zł");

    ui->incomesLabel->setText(formattedIncomes + " zł");

    ui->expensesLabel->setText(formattedExpenses + " zł");

    ui->balanceLabel->setText(formattedBalance + " zł");

    updateTable();
    createPieChart();
    updateAllTasks();
    createLineChart();
    updateDayTasks();
    updateTodayTasks();
}

Dashboard::~Dashboard()
{
    delete ui;
}

void Dashboard::updateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    ui->dateLabel->setText(formattedDateTime);
    ui->dateLabel_2->setText(formattedDateTime);
    ui->dateLabel_3->setText(formattedDateTime);
    ui->dateLabel_4->setText(formattedDateTime);
}

void Dashboard::openQuitDialog() {
    QuitDialog *quitDialog = new QuitDialog;
    quitDialog->show();
}

void Dashboard::on_quitButton_clicked()
{
    openQuitDialog();
}


void Dashboard::on_quitButton_2_clicked()
{
    openQuitDialog();
}


void Dashboard::on_quitButton_3_clicked()
{
    openQuitDialog();
}


void Dashboard::on_quitButton_4_clicked()
{
    openQuitDialog();
}


void Dashboard::on_financesButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Dashboard::on_tasksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void Dashboard::on_accountButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Dashboard::on_dashboardButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Dashboard::on_tasksButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void Dashboard::on_accountButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Dashboard::on_dashboardButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Dashboard::on_financesButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Dashboard::on_accountButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Dashboard::on_dashboardButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Dashboard::on_financesButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Dashboard::on_tasksButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Dashboard::on_deleteAccountButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deleting account", "Are you sure to delete this account? This operation is irreversible", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("hmdb.db");
        db.open();
        QSqlQuery query;
        query.prepare("SELECT userid FROM users WHERE username = :username");
        query.bindValue(":username", username);
        if (query.exec() && query.next()) {
            userid = query.value(0).toInt();
        }
        query.prepare("DELETE FROM finances WHERE userid = :userid");
        query.bindValue(":userid", userid);
        query.exec();
        query.prepare("DELETE FROM tasks WHERE userid = :userid");
        query.bindValue(":userid", userid);
        query.exec();
        query.prepare("DELETE FROM users WHERE username = :username");
        query.bindValue(":username", username);
        if (query.exec()) {
            this->close();
            StartWindow *startWindow = new StartWindow;
            startWindow->show();
        }
    }
}

void Dashboard::on_editAccountButton_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT userid FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        userid = query.value(0).toInt();
    }
    account *editAccount = new account(nullptr, userid);
    connect(editAccount, &account::updateAccount, this, &Dashboard::updateBudget);
    connect(editAccount, &account::updateAccount, this, &Dashboard::updatePieChart);
    connect(editAccount, &account::updateAccount, this, &Dashboard::updateLineChart);
    connect(editAccount, &account::updateAccount, this, &Dashboard::updateUserInfo);
    editAccount->show();
}

void Dashboard::updateUserInfo(int userID) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT username, email, password FROM users WHERE userid = :userid");
    query.bindValue(":userid", userID);
    if (query.exec() && query.next()) {
        username = query.value(0).toString();
        email = query.value(1).toString();
        password = query.value(2).toString();
    }
    ui->welcomeLabel->setText("Welcome " + username + "!");
    ui->welcomeLabel_3->setText("Welcome " + username + "!");
    ui->welcomeLabel_2->setText("Welcome " + username + "!");
    ui->welcomeLabel_4->setText("Welcome " + username + "!");

    ui->usernameLabel_2->setText(username);
    ui->emailLabel_6->setText(email);
    ui->passwordLabel_2->setText("**********");
}

void Dashboard::on_addFinanceButton_clicked()
{
    addFinance *addFinance = new class addFinance(nullptr, id, 0);
    connect(addFinance, &addFinance::transactionAdded, this, &Dashboard::updateTable);
    connect(addFinance, &addFinance::transactionAdded, this, &Dashboard::updateFinances);
    connect(addFinance, &addFinance::transactionAdded, this, &Dashboard::updatePieChart);
    connect(addFinance, &addFinance::transactionAdded, this, &Dashboard::updateLineChart);
    addFinance->show();
}

void Dashboard::updateTable() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("finances");
    QString filter = QString("userid = %1").arg(id);
    model->setFilter(filter);
    model->select();

    ui->financeTable->setModel(model);
    model->setHeaderData(model->fieldIndex("type"), Qt::Horizontal, tr("Type"));
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
    model->setHeaderData(model->fieldIndex("amount"), Qt::Horizontal, tr("Amount"));
    model->setHeaderData(model->fieldIndex("category"), Qt::Horizontal, tr("Category"));
    model->setHeaderData(model->fieldIndex("date"), Qt::Horizontal, tr("Date"));

    ui->financeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->financeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->financeTable->hideColumn(model->fieldIndex("financeid"));
    ui->financeTable->hideColumn(model->fieldIndex("userid"));
}

void Dashboard::updateAllTasks() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();

    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("tasks");
    QString filter = QString("userid = %1").arg(id);
    model->setFilter(filter);
    model->select();

    updateTableView(model, ui->allTasksView, ui->titleTaskLabel, ui->dateTaskLabel, ui->importanceTaskLabel, ui->doneTaskLabel, ui->descriptionTaskLabel);

    QStandardItemModel *standardModel = qobject_cast<QStandardItemModel*>(ui->allTasksView->model());
    if (standardModel) {
        QStandardItem *itemYes = new QStandardItem(tr("Yes"));
        QStandardItem *itemNo = new QStandardItem(tr("No"));
        QStandardItem *item;
        for (int row = 0; row < standardModel->rowCount(); ++row) {
            QModelIndex index = standardModel->index(row, model->fieldIndex("Done"));
            item = standardModel->itemFromIndex(index);
            if (item && item->text() == "1")
                standardModel->setItem(row, model->fieldIndex("Done"), itemYes);
            else if (item && item->text() == "0")
                standardModel->setItem(row, model->fieldIndex("Done"), itemNo);
        }
    }
}

void Dashboard::updateDayTasks() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();

    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("tasks");
    QString filter = QString("userid = %1").arg(id);
    model->setFilter(filter);
    model->select();
    QDate today = QDate::currentDate();
    QString queryStr;

    if (ui->dayBox->currentIndex() == 0) { // Yesterday
        QDate yesterday = today.addDays(-1);
        QString yesterdayStr = yesterday.toString("yyyy-MM-dd");
        queryStr = QString("date = '%1'").arg(yesterdayStr);
    } else if (ui->dayBox->currentIndex() == 1) { // Today
        QString todayStr = today.toString("yyyy-MM-dd");
        queryStr = QString("date = '%1'").arg(todayStr);
    } else if (ui->dayBox->currentIndex() == 2) { // Tommorow
        QDate tomorrow = today.addDays(1);
        QString tomorrowStr = tomorrow.toString("yyyy-MM-dd");
        queryStr = QString("date = '%1'").arg(tomorrowStr);
    }
    model->setFilter(queryStr);

    updateTableView(model, ui->dayTaskView, ui->titleTaskLabel, ui->dateTaskLabel, ui->importanceTaskLabel, ui->doneTaskLabel, ui->descriptionTaskLabel);
}

void Dashboard::updateTableView(QSqlTableModel *model, QTableView *tableView, QLabel *titleLabel, QLabel *dateLabel, QLabel *importanceLabel, QLabel *doneLabel, QLabel *descriptionLabel) {
    model->select();

    tableView->setModel(model);
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
    model->setHeaderData(model->fieldIndex("date"), Qt::Horizontal, tr("Date"));
    model->setHeaderData(model->fieldIndex("importance"), Qt::Horizontal, tr("Importance"));
    model->setHeaderData(model->fieldIndex("done"), Qt::Horizontal, tr("Done"));

    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->hideColumn(model->fieldIndex("taskid"));
    tableView->hideColumn(model->fieldIndex("userid"));
    tableView->hideColumn(model->fieldIndex("description"));

    connect(tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            [this, model, titleLabel, dateLabel, importanceLabel, doneLabel, descriptionLabel](const QModelIndex &current, const QModelIndex &previous) {
                Q_UNUSED(previous);
                if (current.isValid()) {
                    QSqlRecord record = model->record(current.row());
                    titleLabel->setText(record.value("title").toString());
                    dateLabel->setText(record.value("date").toString());
                    importanceLabel->setText(record.value("importance").toString());
                    bool doneValue = record.value("done").toBool();
                    doneLabel->setText(doneValue ? "Yes" : "No");
                    descriptionLabel->setText(record.value("description").toString());
                }
            });

    int totalWidth = ui->horizontalFrameTasks2->width();
    int titleWidth = totalWidth * 0.4;
    int dateWidth = totalWidth * 0.2;
    int narrowWidth = totalWidth * 0.1;

    // Ustawienie szerokości kolumn
    tableView->setColumnWidth(model->fieldIndex("title"), titleWidth);
    tableView->setColumnWidth(model->fieldIndex("date"), dateWidth);
    tableView->setColumnWidth(model->fieldIndex("importance"), dateWidth);
    tableView->setColumnWidth(model->fieldIndex("done"), narrowWidth);
}

void Dashboard::updateTodayTasks() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("tasks");
    QString filter = QString("userid = %1").arg(id);
    model->setFilter(filter);
    model->select();

    QDate today = QDate::currentDate();
    QString queryStr;

    QString todayStr = today.toString("yyyy-MM-dd");
    queryStr = QString("date = '%1'").arg(todayStr);
    model->setFilter(queryStr);

    model->setSort(model->fieldIndex("done"), Qt::AscendingOrder);

    model->select();

    int totalWidth = ui->horizontalFrameTasks3->width();

    int titleWidth = totalWidth * 0.3;
    int doneWidth = totalWidth * 0.2;
    int importanceWidth = totalWidth * 0.3;

    ui->dayTaskView_2->setModel(model);
    ui->dayTaskView_2->setColumnHidden(model->fieldIndex("taskid"), true);
    ui->dayTaskView_2->setColumnHidden(model->fieldIndex("userid"), true);
    ui->dayTaskView_2->setColumnHidden(model->fieldIndex("description"), true);
    ui->dayTaskView_2->setColumnHidden(model->fieldIndex("date"), true);
    ui->dayTaskView_2->setColumnWidth(model->fieldIndex("title"), titleWidth);
    ui->dayTaskView_2->setColumnWidth(model->fieldIndex("done"), doneWidth);
    ui->dayTaskView_2->setColumnWidth(model->fieldIndex("importance"), importanceWidth);

    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
    model->setHeaderData(model->fieldIndex("done"), Qt::Horizontal, tr("Done"));
    model->setHeaderData(model->fieldIndex("importance"), Qt::Horizontal, tr("Importance"));

    QStandardItemModel *standardModel = qobject_cast<QStandardItemModel*>(ui->dayTaskView_2->model());
    if (standardModel) {
        QStandardItem *itemYes = new QStandardItem(tr("Yes"));
        QStandardItem *itemNo = new QStandardItem(tr("No"));
        QStandardItem *item;
        for (int row = 0; row < standardModel->rowCount(); ++row) {
            QModelIndex index = standardModel->index(row, model->fieldIndex("done"));
            item = standardModel->itemFromIndex(index);
            if (item && item->text() == "1")
                standardModel->setItem(row, model->fieldIndex("done"), itemYes);
            else if (item && item->text() == "0")
                standardModel->setItem(row, model->fieldIndex("done"), itemNo);
        }
    }

    ui->dayTaskView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dayTaskView_2->setSelectionMode(QAbstractItemView::SingleSelection);
}


void Dashboard::on_dayBox_currentIndexChanged(int index)
{
    updateDayTasks();
}

void Dashboard::on_passwordCheckBox_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE userid = :userid");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        password = query.value(0).toString();
    }
    if(ui->passwordCheckBox->isChecked()) ui->passwordLabel_2->setText(password);
    else ui->passwordLabel_2->setText("**********");
}

void Dashboard::createPieChart() {
    QPieSeries *series = new QPieSeries();
    series->append("Incomes", incomes)->setBrush(Qt::green);
    series->append("Expenses", expenses)->setBrush(Qt::red);

    QChart *chart = new QChart();
    chart->addSeries(series);

    chart->legend()->setColor(Qt::white);
    chart->setTitle("Comparison of Incomes and Expenses");
    QChartView *chartView = new QChartView(chart);
    ui->chartFrame->layout()->addWidget(chartView);
    chart->setBackgroundRoundness(0);
    chart->setBackgroundBrush(Qt::NoBrush);
}

void Dashboard::updateFinances(int id) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;

    query.prepare("SELECT type, amount FROM finances WHERE userid = :userid ORDER BY financeid DESC LIMIT 1");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        type = query.value(0).toString();
        amount = query.value(1).toFloat();
        qDebug() << type;
        if (type == "Income") budget += amount;
        else if (type == "Expense") budget -= amount;
    }

    query.prepare("UPDATE users SET budget = :budget WHERE userid = :userid");
    query.bindValue(":budget", budget);
    query.bindValue(":userid", id);
    query.exec();

    query.prepare("SELECT budget FROM users WHERE userid = :userid");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        budget = query.value(0).toFloat();
        formattedBudget = QString::number(budget, 'f', 2);
    }

    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Income' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        incomes = query.value(0).toFloat();
        formattedIncomes = QString::number(incomes, 'f', 2);
    }

    // Dla wydatków (Expense)
    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Expense' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        expenses = query.value(0).toFloat();
        formattedExpenses = QString::number(expenses, 'f', 2);
    }

    balance = incomes - expenses;
    formattedBalance = QString::number(balance, 'f', 2);

    ui->budgetLabel->setText(formattedBudget + " zł");

    ui->incomesLabel->setText(formattedIncomes + " zł");

    ui->expensesLabel->setText(formattedExpenses + " zł");

    ui->balanceLabel->setText(formattedBalance + " zł");
}

void Dashboard::updateBudget(int id) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT budget FROM users WHERE userid = :userid");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        budget = query.value(0).toFloat();
        formattedBudget = QString::number(budget, 'f', 2);
    }

    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Income' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        incomes = query.value(0).toFloat();
        formattedIncomes = QString::number(incomes, 'f', 2);
    }

    // Dla wydatków (Expense)
    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Expense' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        expenses = query.value(0).toFloat();
        formattedExpenses = QString::number(expenses, 'f', 2);
    }

    balance = incomes - expenses;
    formattedBalance = QString::number(balance, 'f', 2);

    ui->budgetLabel->setText(formattedBudget + " zł");

    ui->incomesLabel->setText(formattedIncomes + " zł");

    ui->expensesLabel->setText(formattedExpenses + " zł");

    ui->balanceLabel->setText(formattedBalance + " zł");
}

void Dashboard::updateEditedFinances(int id, int financeID) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT budget FROM users WHERE userid = :userid");
    query.bindValue(":userid", id);
    if(query.exec() && query.next()) {
        budget = query.value(0).toFloat();
    }
    query.prepare("SELECT type, amount FROM finances WHERE userid = :userid AND financeid = :financeid ORDER BY financeid DESC LIMIT 1");
    query.bindValue(":userid", id);
    query.bindValue(":financeid", financeID);
    if (query.exec() && query.next()) {
        type = query.value(0).toString();
        amount = query.value(1).toFloat();
        if (type == oldType) {
            if (amount > oldAmount) budget += (amount - oldAmount);
            else if (amount < oldAmount) budget -= (oldAmount - amount);
        }
        else if (type == "Income" && oldType == "Expense") budget += (oldAmount + amount);
        else if (type == "Expense" && oldType == "Income") budget -= (oldAmount + amount);
    }
    qDebug() << budget;
    query.prepare("UPDATE users SET budget = :budget WHERE userid = :userid");
    query.bindValue(":budget", budget);
    query.bindValue(":userid", id);
    query.exec();

    query.prepare("SELECT budget FROM users WHERE userid = :userid");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        budget = query.value(0).toFloat();
        formattedBudget = QString::number(budget, 'f', 2);
    }

    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Income' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        incomes = query.value(0).toFloat();
        formattedIncomes = QString::number(incomes, 'f', 2);
    }

    // Dla wydatków (Expense)
    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Expense' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        expenses = query.value(0).toFloat();
        formattedExpenses = QString::number(expenses, 'f', 2);
    }

    balance = incomes - expenses;
    formattedBalance = QString::number(balance, 'f', 2);

    ui->budgetLabel->setText(formattedBudget + " zł");

    ui->incomesLabel->setText(formattedIncomes + " zł");

    ui->expensesLabel->setText(formattedExpenses + " zł");

    ui->balanceLabel->setText(formattedBalance + " zł");
}

void Dashboard::updateRemoveFinances(int id) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;

    query.prepare("SELECT budget FROM users WHERE userid = :userid");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        budget = query.value(0).toFloat();
        formattedBudget = QString::number(budget, 'f', 2);
    }

    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Income' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        incomes = query.value(0).toFloat();
        formattedIncomes = QString::number(incomes, 'f', 2);
    }

    // Dla wydatków (Expense)
    query.prepare("SELECT COALESCE(SUM(amount), 0.00) FROM finances WHERE type = 'Expense' AND userid = :userid AND strftime('%Y-%m', date) = strftime('%Y-%m', 'now')");
    query.bindValue(":userid", id);
    if (query.exec() && query.next()) {
        expenses = query.value(0).toFloat();
        formattedExpenses = QString::number(expenses, 'f', 2);
    }

    balance = incomes - expenses;
    formattedBalance = QString::number(balance, 'f', 2);

    ui->budgetLabel->setText(formattedBudget + " zł");

    ui->incomesLabel->setText(formattedIncomes + " zł");

    ui->expensesLabel->setText(formattedExpenses + " zł");

    ui->balanceLabel->setText(formattedBalance + " zł");
}

void Dashboard::updatePieChart() {
    QChartView *chartView = qobject_cast<QChartView*>(ui->chartFrame->layout()->itemAt(0)->widget());
    if (!chartView) {
        qDebug() << "Chart view not found!";
        return;
    }
    QChart *chart = chartView->chart();
    if (!chart) {
        qDebug() << "Chart not found!";
        return;
    }

    QPieSeries *series = dynamic_cast<QPieSeries*>(chart->series().at(0));
    if (series) {
        series->clear();
        series->append("Incomes", incomes)->setBrush(Qt::green);
        series->append("Expenses", expenses)->setBrush(Qt::red);
    } else {
        qDebug() << "Series not found!";
        return;
    }
    chart->setBackgroundRoundness(0);
    chart->setBackgroundBrush(Qt::NoBrush);
}

void Dashboard::createLineChart() {
    QStringList months = getLast6Months();
    QVector<double> incomeSums = getTransactionSumsByType("Income", id);
    QVector<double> expenseSums = getTransactionSumsByType("Expense", id);

    QLineSeries *incomeSeries = new QLineSeries();
    incomeSeries->setName("Incomes");
    for (int i = 0; i < months.size(); i++) {
        incomeSeries->append(i, incomeSums[i]);
    }

    QLineSeries *expenseSeries = new QLineSeries();
    expenseSeries->setName("Expenses");
    for (int i = 0; i < months.size(); i++) {
        expenseSeries->append(i, expenseSums[i]);
    }

    QChart *chart = new QChart();
    chart->removeAllSeries();

    chart->addSeries(incomeSeries);
    chart->addSeries(expenseSeries);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(months);
    chart->addAxis(axisX, Qt::AlignBottom);
    incomeSeries->attachAxis(axisX);
    expenseSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Amount");

    double max = qMax(*std::max_element(incomeSums.constBegin(), incomeSums.constEnd()),
                      *std::max_element(expenseSums.constBegin(), expenseSums.constEnd()));
    double min = qMin(*std::min_element(incomeSums.constBegin(), incomeSums.constEnd()),
                      *std::min_element(expenseSums.constBegin(), expenseSums.constEnd()));

    axisY->setRange(min * 0.9, max * 1.1);

    chart->addAxis(axisY, Qt::AlignLeft);
    incomeSeries->attachAxis(axisY);
    expenseSeries->attachAxis(axisY);
    chart->setTitle("Comparison of Incomes and Expenses in last 6 months");
    QChartView *chartView = new QChartView(chart);
    ui->chartFrame_2->layout()->addWidget(chartView);
}

void Dashboard::updateLineChart() {
    QStringList months = getLast6Months();
    QVector<double> incomeSums = getTransactionSumsByType("Income", id);
    QVector<double> expenseSums = getTransactionSumsByType("Expense", id);

    QChartView *chartView = qobject_cast<QChartView*>(ui->chartFrame_2->layout()->itemAt(0)->widget());
    if (!chartView) {
        qDebug() << "Chart view not found.";
        return;
    }

    QChart *chart = chartView->chart();
    if (!chart) {
        qDebug() << "Chart not found.";
        return;
    }

    QList<QAbstractSeries*> seriesList = chart->series();
    if (seriesList.size() < 2) {
        qDebug() << "Not enough series found.";
        return;
    }

    QLineSeries *incomeSeries = qobject_cast<QLineSeries*>(seriesList.at(0));
    QLineSeries *expenseSeries = qobject_cast<QLineSeries*>(seriesList.at(1));

    if (!incomeSeries || !expenseSeries) {
        qDebug() << "Series type mismatch.";
        return;
    }

    incomeSeries->clear();
    for (int i = 0; i < months.size(); i++) {
        incomeSeries->append(i, incomeSums[i]);
    }

    expenseSeries->clear();
    for (int i = 0; i < months.size(); i++) {
        expenseSeries->append(i, expenseSums[i]);
    }
}


QStringList Dashboard::getLast6Months() {
    QStringList months;
    QDate currentDate = QDate::currentDate();

    for (int i = 5; i >= 0; i--) {
        QDate month = currentDate.addMonths(-i);
        QString formattedMonth = month.toString("yyyy-MM");
        months.append(formattedMonth);
    }

    return months;
}

QVector<double> Dashboard::getTransactionSumsByType(const QString& transactionType, int userId) {
    QVector<double> sums;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Database connection error.";
        return sums;
    }

    for (const QString& month : getLast6Months()) {
        double sum = 0.0;

        QSqlQuery query;
        query.prepare("SELECT SUM(amount) FROM finances WHERE type = :type AND date LIKE :date AND userid = :userid");
        query.bindValue(":type", transactionType);
        query.bindValue(":date", month + "%");
        query.bindValue(":userid", userId);

        if (query.exec() && query.next()) {
            sum = query.value(0).toDouble();
        } else {
            qDebug() << "Query execution error for month:" << month;
        }

        sums.append(sum);
    }

    return sums;
}

void Dashboard::on_editFinanceButton_clicked()
{
    QModelIndexList selection = ui->financeTable->selectionModel()->selectedRows();
    if (selection.empty()) {
        QMessageBox::warning(this, "Warning", "Please select row to edit");
        return;
    }

    QModelIndex index = selection.at(0);
    int row = index.row();

    int financeID = ui->financeTable->model()->data(ui->financeTable->model()->index(row, 0)).toInt();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT type, amount FROM finances WHERE userid = :userid AND financeid = :financeid");
    query.bindValue(":userid", id);
    query.bindValue(":financeid", financeID);
    if (query.exec() && query.next()) {
        oldType = query.value(0).toString();
        oldAmount = query.value(1).toFloat();
    }
    addFinance *addFinance = new class addFinance(nullptr, id, 1, financeID);
    connect(addFinance, &addFinance::transactionUpdated, this, &Dashboard::updateTable);
    connect(addFinance, &addFinance::transactionUpdated, this, &Dashboard::updateEditedFinances);
    connect(addFinance, &addFinance::transactionUpdated, this, &Dashboard::updatePieChart);
    connect(addFinance, &addFinance::transactionUpdated, this, &Dashboard::updateLineChart);
    addFinance->show();

}


void Dashboard::on_removeFinanceButton_clicked()
{
    QModelIndexList selection = ui->financeTable->selectionModel()->selectedRows();
    if (selection.empty()) {
        QMessageBox::warning(this, "Warning", "Please select row to edit");
        return;
    }

    QModelIndex index = selection.at(0);
    int row = index.row();

    int financeID = ui->financeTable->model()->data(ui->financeTable->model()->index(row, 0)).toInt();


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT userid FROM finances WHERE financeid = :financeid");
    query.bindValue(":financeid", financeID);
    if(query.exec() && query.next()) {
        userid = query.value(0).toInt();
    }
    query.prepare("DELETE FROM finances WHERE financeid = :financeid");
    query.bindValue(":financeid", financeID);
    query.exec();
        updateTable();
        updateRemoveFinances(userid);
}


void Dashboard::on_addTaskButton_clicked()
{
    task *addTask = new task(nullptr, id, 0);
    connect(addTask, &task::taskAdded, this, &Dashboard::updateAllTasks);
    connect(addTask, &task::taskAdded, this, &Dashboard::updateDayTasks);
    connect(addTask, &task::taskAdded, this, &Dashboard::updateTodayTasks);
    addTask->show();

}


void Dashboard::on_editTaskButton_clicked()
{
    QModelIndexList selection = ui->allTasksView->selectionModel()->selectedRows();
    if (selection.empty()) {
        QMessageBox::warning(this, "Warning", "Please select row to edit");
        return;
    }

    QModelIndex index = selection.at(0);
    int row = index.row();

    int taskID = ui->allTasksView->model()->data(ui->allTasksView->model()->index(row, 0)).toInt();
    task *addTask = new task(nullptr, id, 1, taskID);
    connect(addTask, &task::taskUpdated, this, &Dashboard::updateAllTasks);
    connect(addTask, &task::taskUpdated, this, &Dashboard::updateDayTasks);
    connect(addTask, &task::taskUpdated, this, &Dashboard::updateTodayTasks);
    addTask->show();
    ui->titleTaskLabel->setText("Title");
    ui->dateTaskLabel->setText("Date");
    ui->importanceTaskLabel->setText("★");
    ui->doneTaskLabel->setText("✔");
    ui->descriptionTaskLabel->setText("Desciption");
}


void Dashboard::on_removeTaskButton_clicked()
{
    QModelIndexList selection = ui->allTasksView->selectionModel()->selectedRows();
    if (selection.empty()) {
        QMessageBox::warning(this, "Warning", "Please select row to edit");
        return;
    }

    QModelIndex index = selection.at(0);
    int row = index.row();

    int taskID = ui->allTasksView->model()->data(ui->allTasksView->model()->index(row, 0)).toInt();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT userid FROM tasks WHERE taskid = :taskid");
    query.bindValue(":taskid", taskID);
    if(query.exec() && query.next())
    {
        userid = query.value(0).toInt();
    }
    query.prepare("DELETE FROM tasks WHERE taskid = :taskid");
    query.bindValue(":taskid", taskID);
    query.exec();
    updateAllTasks();
    updateDayTasks();
    updateTodayTasks();
    ui->titleTaskLabel->setText("Title");
    ui->dateTaskLabel->setText("Date");
    ui->importanceTaskLabel->setText("★");
    ui->doneTaskLabel->setText("✔");
    ui->descriptionTaskLabel->setText("Desciption");
}

void Dashboard::on_makeAsDoneButton_clicked()
{
    QModelIndexList selection = ui->allTasksView->selectionModel()->selectedRows();
    if (selection.empty()) {
        QMessageBox::warning(this, "Warning", "Please select row to edit");
        return;
    }

    QModelIndex index = selection.at(0);
    int row = index.row();

    int taskID = ui->allTasksView->model()->data(ui->allTasksView->model()->index(row, 0)).toInt();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT done FROM tasks WHERE taskid = :taskid");
    query.bindValue(":taskid", taskID);
    if(query.exec() && query.next())
    {
        bool isDone = query.value(0).toBool();
        if (isDone == 1) return;
        else if (isDone == 0) {
            query.prepare("UPDATE tasks SET done = :done WHERE taskid = :taskid");
            query.bindValue(":taskid", taskID);
            query.bindValue(":done", true);
            query.exec();
        }
    }
    updateAllTasks();
    updateDayTasks();
    updateTodayTasks();
    ui->titleTaskLabel->setText("Title");
    ui->dateTaskLabel->setText("Date");
    ui->importanceTaskLabel->setText("★");
    ui->doneTaskLabel->setText("✔");
    ui->descriptionTaskLabel->setText("Desciption");
}


void Dashboard::on_makeAsUndoneButton_clicked()
{
    QModelIndexList selection = ui->allTasksView->selectionModel()->selectedRows();
    if (selection.empty()) {
        QMessageBox::warning(this, "Warning", "Please select row to edit");
        return;
    }

    QModelIndex index = selection.at(0);
    int row = index.row();

    int taskID = ui->allTasksView->model()->data(ui->allTasksView->model()->index(row, 0)).toInt();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT done FROM tasks WHERE taskid = :taskid");
    query.bindValue(":taskid", taskID);
    if(query.exec() && query.next())
    {
        bool isDone = query.value(0).toBool();
        if (isDone == 0) return;
        else if (isDone == 1) {
            query.prepare("UPDATE tasks SET done = :done WHERE taskid = :taskid");
            query.bindValue(":taskid", taskID);
            query.bindValue(":done", false);
            query.exec();
        }
    }
    updateAllTasks();
    updateDayTasks();
    updateTodayTasks();
    ui->titleTaskLabel->setText("Title");
    ui->dateTaskLabel->setText("Date");
    ui->importanceTaskLabel->setText("★");
    ui->doneTaskLabel->setText("✔");
    ui->descriptionTaskLabel->setText("Desciption");
}

