#include "quitdialog.h"
#include "ui_quitdialog.h"
#include "startwindow.h"

QuitDialog::QuitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QuitDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Home Management");
    this->setWindowIcon(QIcon(":/Resources/icon.ico"));
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

QuitDialog::~QuitDialog()
{
    delete ui;
}

void QuitDialog::on_logoutButton_clicked()
{
    const auto windows = QApplication::topLevelWidgets();
    for (QWidget *window : windows) {
        if (window->isWindow() && window->isVisible()) {
            window->close();
        }
    }
    StartWindow *startWindow = new StartWindow;
    startWindow->show();
}


void QuitDialog::on_closeButton_clicked()
{
    qApp->quit();
}
