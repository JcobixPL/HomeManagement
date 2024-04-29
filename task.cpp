#include "task.h"
#include "ui_task.h"

task::task(QWidget *parent, int userID, int pageID, int taskID)
    : QDialog(parent)
    , ui(new Ui::task)
{
    ui->setupUi(this);
    this->setWindowTitle("Home Management");
    this->setWindowIcon(QIcon(":/Resources/icon.ico"));
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    id = userID;
    taskid = taskID;
    if (pageID == 1) setParametersToEdit(taskID);
    ui->stackedWidget->setCurrentIndex(pageID);
    ui->dateEdit_3->setDate(QDate::currentDate());
}

task::~task()
{
    delete ui;
}

void task::setParametersToEdit(int taskID) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("SELECT title, date, done, importance, description FROM tasks WHERE taskid = :taskid");
    query.bindValue(":taskid", taskID);
    if (query.exec() && query.next()) {
        if (query.value(2).toBool() == false) ui->doneComboBox_2->setCurrentIndex(0);
        else ui->doneComboBox_2->setCurrentIndex(1);
        if (query.value(3).toInt() == 1) ui->importanceSpinBox_2->setValue(1);
        else if (query.value(3).toInt() == 2) ui->importanceSpinBox_2->setValue(2);
        else ui->importanceSpinBox_2->setValue(3);
        ui->titleLineEdit_5->setText(query.value(0).toString());
        ui->dateEdit_4->setDate(query.value(1).toDate());
        ui->descriptionLineEdit_2->setText(query.value(4).toString());
    }
}

void task::on_confirmTaskButton_clicked()
{
    title = ui->titleLineEdit_4->text();
    date = ui->dateEdit_3->date();
    importance = ui->importanceSpinBox->value();
    description = ui->descriptionLineEdit->text();
    done = ui->doneComboBox->currentText();
    bool isDone = (done == "Yes") ? 1 : 0;

    if (title.isEmpty() || description.isEmpty()) {
        QMessageBox::warning(this, "Warning", "All fields need to be filled in");
        return;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();

    QSqlQuery query;
    query.prepare("INSERT INTO tasks (userid, title, date, done, importance, description) VALUES (:userid, :title, :date, :done, :importance, :description)");
    query.bindValue(":userid", id);
    query.bindValue(":title", title);
    query.bindValue(":date", date);
    query.bindValue(":done", isDone);
    query.bindValue(":importance", importance);
    query.bindValue(":description", description);
    query.exec();
    db.close();
    emit taskAdded(id);
    this->close();
}

void task::on_confirmTaskButton_2_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    title = ui->titleLineEdit_5->text();
    date = ui->dateEdit_4->date();
    importance = ui->importanceSpinBox_2->value();
    done = ui->doneComboBox_2->currentText();
    description = ui->descriptionLineEdit_2->text();
    bool isDone = (done == "Yes") ? 1 : 0;

    if (title.isEmpty() || description.isEmpty()) {
        QMessageBox::warning(this, "Warning", "All fields need to be filled in");
        return;
    }
    qDebug() << "taskid = " << taskid;
    qDebug() << "userid = " << id;
    QSqlQuery query;
    query.prepare("UPDATE tasks SET title = :title, date = :date, importance = :importance, done = :done, description = :description WHERE taskid = :taskid");
    query.bindValue(":title", title);
    query.bindValue(":date", date);
    query.bindValue(":done", isDone);
    query.bindValue(":importance", importance);
    query.bindValue(":description", description);
    query.bindValue(":taskid", taskid);
    query.exec();
    db.close();
    emit taskUpdated(id, taskid);
    this->close();
}

