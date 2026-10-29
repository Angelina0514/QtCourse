#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    setWindowTitle("替换");
    setFixedSize(width(), height());

    connect(ui->btnFindNext, &QPushButton::clicked, this, &ReplaceDialog::onFindNextClicked);
    connect(ui->btnReplace, &QPushButton::clicked, this, &ReplaceDialog::onReplaceClicked);
    connect(ui->btnReplaceAll, &QPushButton::clicked, this, &ReplaceDialog::onReplaceAllClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::hide);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::onFindNextClicked()
{
    QString target = ui->editFind->text();
    if (target.isEmpty()) {
        QMessageBox::information(this, "提示", "查找目标不能为空！");
        return;
    }
    emit findNext(target, ui->rbDown->isChecked(), ui->cbCase->isChecked());
}

void ReplaceDialog::onReplaceClicked()
{
    QString target = ui->editFind->text();
    if (target.isEmpty()) {
        QMessageBox::information(this, "提示", "查找目标不能为空！");
        return;
    }
    emit replaceOne(target, ui->editReplace->text(), ui->rbDown->isChecked(), ui->cbCase->isChecked());
}

void ReplaceDialog::onReplaceAllClicked()
{
    QString target = ui->editFind->text();
    if (target.isEmpty()) {
        QMessageBox::information(this, "提示", "查找目标不能为空！");
        return;
    }
    emit replaceAll(target, ui->editReplace->text(), ui->cbCase->isChecked());
}
