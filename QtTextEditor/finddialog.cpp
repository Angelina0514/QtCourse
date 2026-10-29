#include "finddialog.h"
#include "ui_finddialog.h"
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    setWindowTitle("查找");
    setFixedSize(width(), height());

    connect(ui->btnFindNext, &QPushButton::clicked, this, &FindDialog::onFindNextClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::hide);
}

FindDialog::~FindDialog()
{
    delete ui;
}

QString FindDialog::targetText() const
{
    return ui->editTarget->text();
}

bool FindDialog::isDownDirection() const
{
    return ui->rbDown->isChecked();
}

bool FindDialog::isCaseSensitive() const
{
    return ui->cbCase->isChecked();
}

void FindDialog::onFindNextClicked()
{
    QString text = ui->editTarget->text();
    if (text.isEmpty()) {
        QMessageBox::information(this, "提示", "查找目标不能为空！");
        return;
    }
    emit findNext(text, ui->rbDown->isChecked(), ui->cbCase->isChecked());
}
