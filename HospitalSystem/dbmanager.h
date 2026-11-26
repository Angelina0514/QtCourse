#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QString>

// 数据库管理器（单例）：负责连接、建表、登录验证
class DatabaseManager
{
public:
    static DatabaseManager &instance();

    bool open(const QString &dbPath = "hospital.db");  // 打开数据库
    void close();
    bool isOpen() const;
    QString lastError() const;

    bool initDatabase();                                // 建表 + 初始数据
    bool verifyUser(const QString &user, const QString &pass, QString &role);

private:
    DatabaseManager() = default;
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    QSqlDatabase m_db;
    QString m_lastError;
};

#endif // DBMANAGER_H
