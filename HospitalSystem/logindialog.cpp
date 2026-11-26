#include "logindialog.h"
#include "dbmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("用户登录");
    setFixedSize(320, 190);

    m_userEdit = new QLineEdit(this);
    m_passEdit = new QLineEdit(this);
    m_passEdit->setEchoMode(QLineEdit::Password);

    m_hintLabel = new QLabel("默认账号：admin / 123456", this);
    m_hintLabel->setStyleSheet("color: gray;");

    auto *loginBtn = new QPushButton("登录", this);
    auto *cancelBtn = new QPushButton("退出", this);
    loginBtn->setDefault(true);

    auto *form = new QFormLayout;
    form->addRow("用户名：", m_userEdit);
    form->addRow("密　码：", m_passEdit);

    auto *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    btnRow->addWidget(loginBtn);
    btnRow->addWidget(cancelBtn);

    auto *lay = new QVBoxLayout(this);
    lay->addLayout(form);
    lay->addWidget(m_hintLabel);
    lay->addLayout(btnRow);

    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QString LoginDialog::username() const
{
    return m_userEdit->text().trimmed();
}

void LoginDialog::onLogin()
{
    QString user = m_userEdit->text().trimmed();
    QString pass = m_passEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名和密码不能为空！");
        return;
    }

    QString role;
    if (DatabaseManager::instance().verifyUser(user, pass, role)) {
        accept();   // 登录成功，关闭对话框
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
        m_passEdit->clear();
        m_passEdit->setFocus();
    }
}
