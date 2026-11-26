#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListWidget;
class QStackedWidget;
class QTableView;
class QLineEdit;
class QSqlTableModel;
class QSqlQueryModel;

// 主窗口：QStackedWidget 实现页面切换，QTableView + QSqlTableModel 实现列表显示
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &loginUser, QWidget *parent = nullptr);

private slots:
    void switchPage(int row);       // 切换页面
    void addPatient();
    void editPatient();
    void deletePatient();
    void searchPatient();
    void refreshPatient();
    void addDoctor();
    void deleteDoctor();
    void addDepartment();
    void deleteDepartment();
    void refreshLog();

private:
    void setupUi();
    void setupPatientPage();
    void setupDoctorPage();
    void setupDepartmentPage();
    void setupLogPage();
    void log(const QString &action, const QString &detail);  // 写操作日志

    QString m_user;                 // 登录用户名
    QListWidget *m_nav;             // 左侧导航
    QStackedWidget *m_stack;        // 右侧页面堆

    QTableView *m_patientView;
    QSqlTableModel *m_patientModel;
    QLineEdit *m_searchEdit;

    QTableView *m_doctorView;
    QSqlTableModel *m_doctorModel;

    QTableView *m_deptView;
    QSqlTableModel *m_deptModel;

    QTableView *m_logView;
    QSqlQueryModel *m_logModel;
};

#endif // MAINWINDOW_H
