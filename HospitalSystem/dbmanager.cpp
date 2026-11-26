#include "dbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QDebug>

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open(const QString &dbPath)
{
    if (QSqlDatabase::contains("main_conn")) {
        m_db = QSqlDatabase::database("main_conn");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "main_conn");
        m_db.setDatabaseName(dbPath);
    }
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::close()
{
    if (m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

bool DatabaseManager::initDatabase()
{
    // 建表语句
    QStringList ddl = {
        "CREATE TABLE IF NOT EXISTS users ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " username VARCHAR(50) NOT NULL UNIQUE,"
        " password VARCHAR(50) NOT NULL,"
        " role VARCHAR(20) DEFAULT 'user')",

        "CREATE TABLE IF NOT EXISTS patients ("
        " patient_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " name VARCHAR(50) NOT NULL,"
        " gender VARCHAR(4),"
        " age INTEGER,"
        " phone VARCHAR(20),"
        " department VARCHAR(50),"
        " doctor VARCHAR(50),"
        " diagnosis VARCHAR(200),"
        " visit_date VARCHAR(20))",

        "CREATE TABLE IF NOT EXISTS departments ("
        " dept_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " dept_name VARCHAR(50) NOT NULL,"
        " location VARCHAR(50))",

        "CREATE TABLE IF NOT EXISTS doctors ("
        " doctor_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " name VARCHAR(50) NOT NULL,"
        " department VARCHAR(50),"
        " title VARCHAR(20))",

        "CREATE TABLE IF NOT EXISTS op_logs ("
        " log_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " operator VARCHAR(50),"
        " action VARCHAR(50),"
        " detail VARCHAR(200),"
        " op_time VARCHAR(20))"
    };

    for (const QString &sql : ddl) {
        QSqlQuery q(m_db);
        if (!q.exec(sql)) {
            m_lastError = q.lastError().text();
            return false;
        }
    }

    // 首次运行插入初始账号
    QSqlQuery cnt(m_db);
    cnt.exec("SELECT COUNT(*) FROM users");
    cnt.next();
    if (cnt.value(0).toInt() == 0) {
        struct Seed { const char *u, *p, *r; };
        const Seed seeds[] = {
            {"admin", "123456", "admin"},
            {"doctor", "123456", "user"},
        };
        for (const Seed &s : seeds) {
            QSqlQuery ins(m_db);
            ins.prepare("INSERT INTO users(username, password, role) VALUES(?, ?, ?)");
            ins.addBindValue(s.u);
            ins.addBindValue(s.p);
            ins.addBindValue(s.r);
            ins.exec();
        }
    }
    return true;
}

bool DatabaseManager::verifyUser(const QString &user, const QString &pass, QString &role)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT role FROM users WHERE username = ? AND password = ?");
    q.addBindValue(user);
    q.addBindValue(pass);
    if (q.exec() && q.next()) {
        role = q.value(0).toString();
        return true;
    }
    return false;
}
