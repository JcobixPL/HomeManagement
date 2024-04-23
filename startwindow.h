#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class StartWindow;
}
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase db;
    QSqlQuery query;
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private slots:
    void on_logInButton_clicked();

    void on_signUpButton_clicked();

private:
    Ui::StartWindow *ui;
};
#endif // STARTWINDOW_H
