#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle("关于小牛文本编辑");
    setFixedSize(width(), height());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_btnOk_clicked()
{
    accept();
}
