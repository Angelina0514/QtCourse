#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

// 登录对话框：验证用户名密码
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString username() const;

private slots:
    void onLogin();

private:
    QLineEdit *m_userEdit;
    QLineEdit *m_passEdit;
    QLabel *m_hintLabel;
};

#endif // LOGINDIALOG_H
