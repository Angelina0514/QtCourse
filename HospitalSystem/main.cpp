#include <QApplication>
#include <QDialog>
#include <QMessageBox>

#include "dbmanager.h"
#include "logindialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 1. 连接数据库，失败给出明确提示
    DatabaseManager &db = DatabaseManager::instance();
    if (!db.open("hospital.db")) {
        QMessageBox::critical(nullptr, "数据库连接失败",
                              "无法打开数据库：\n" + db.lastError());
        return -1;
    }
    if (!db.initDatabase()) {
        QMessageBox::critical(nullptr, "数据库初始化失败", db.lastError());
        return -1;
    }

    // 2. 登录
    LoginDialog login;
    if (login.exec() != QDialog::Accepted)
        return 0;   // 用户退出

    // 3. 进入主界面
    MainWindow w(login.username());
    w.show();
    return app.exec();
}
