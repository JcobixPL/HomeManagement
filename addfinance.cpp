#include "addfinance.h"
#include "ui_addfinance.h"

addFinance::addFinance(QWidget *parent, int userID, int pageID, int financeID)
    : QDialog(parent)
    , ui(new Ui::addFinance)
{
    ui->setupUi(this);
    this->setWindowTitle("Home Management");
    this->setWindowIcon(QIcon(":/Resources/icon.ico"));
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    id = userID;
    financeid = financeID;
    ui->stackedWidget->setCurrentIndex(pageID);
    if (pageID == 1) setParametersToEdit(financeID);
    ui->dateEdit->setDate(QDate::currentDate());

}

addFinance::~addFinance()
{
    delete ui;
}

void addFinance::setParametersToEdit(int financeID) {
    QSqlQuery query;
    query.prepare("SELECT type, amount, title, date, category FROM finances WHERE financeid = :financeId");
    query.bindValue(":financeId", financeID);
    if (query.exec() && query.next()) {
        if (query.value(0).toString() == "Income") ui->typeComboBox_3->setCurrentIndex(0);
        else ui->typeComboBox_3->setCurrentIndex(1);
        ui->amountSpinBox_3->setValue(query.value(1).toFloat());
        ui->titleLineEdit_3->setText(query.value(2).toString());
        ui->dateEdit_2->setDate(query.value(3).toDate());
        ui->categoryLineEdit_3->setText(query.value(4).toString());
    }
}

void addFinance::on_confirmButton_clicked()
{
    type = ui->typeComboBox->currentText();
    amount = ui->amountSpinBox->value();
    title = ui->titleLineEdit->text();
    date = ui->dateEdit->date();
    category = ui->categoryLineEdit->text();

    if (amount == 0 || title.isEmpty() || category.isEmpty()) {
        QMessageBox::warning(this, "Warning", "All fields need to be filled in");
        return;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("INSERT INTO finances (userid, type, amount, title, date, category) VALUES (:userid, :type, :amount, :title, :date, :category)");
    query.bindValue(":userid", id);
    query.bindValue(":type", type);
    query.bindValue(":amount", amount);
    query.bindValue(":title", title);
    query.bindValue(":date", date);
    query.bindValue(":category", category);
    query.exec();
    db.close();
    emit transactionAdded(id);
    this->close();
}

void addFinance::on_confirmButton_3_clicked()
{
    type = ui->typeComboBox_3->currentText();
    amount = ui->amountSpinBox_3->value();
    title = ui->titleLineEdit_3->text();
    date = ui->dateEdit_2->date();
    category = ui->categoryLineEdit_3->text();

    if (amount == 0 || title.isEmpty() || category.isEmpty()) {
        QMessageBox::warning(this, "Warning", "All fields need to be filled in");
        return;
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hmdb.db");
    db.open();
    QSqlQuery query;
    query.prepare("UPDATE finances SET userid = :userid, type = :type, amount = :amount, title = :title, date = :date, category = :category WHERE financeid = :financeID");
    query.bindValue(":userid", id);
    query.bindValue(":type", type);
    query.bindValue(":amount", amount);
    query.bindValue(":title", title);
    query.bindValue(":date", date);
    query.bindValue(":category", category);
    query.bindValue(":financeID", financeid);
    query.exec();
    db.close();
    emit transactionUpdated(id, financeid);
    this->close();
}

