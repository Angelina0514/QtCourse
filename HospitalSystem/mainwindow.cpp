#include "mainwindow.h"
#include "patientdialog.h"
#include "dbmanager.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QHeaderView>
#include <QDateTime>
#include <QAbstractItemView>

MainWindow::MainWindow(const QString &loginUser, QWidget *parent)
    : QMainWindow(parent), m_user(loginUser)
{
    setupUi();
}

void MainWindow::setupUi()
{
    setWindowTitle("医院诊疗系统 - 用户信息管理");
    resize(1000, 640);

    // 左侧导航
    m_nav = new QListWidget(this);
    m_nav->addItems(QStringList() << "患者管理" << "医生管理" << "科室管理" << "操作日志");
    m_nav->setFixedWidth(140);

    // 右侧页面堆
    m_stack = new QStackedWidget(this);
    setupPatientPage();
    setupDoctorPage();
    setupDepartmentPage();
    setupLogPage();

    auto *splitter = new QSplitter(this);
    splitter->addWidget(m_nav);
    splitter->addWidget(m_stack);
    splitter->setStretchFactor(1, 1);
    setCentralWidget(splitter);



    connect(m_nav, &QListWidget::currentRowChanged,
            this, &MainWindow::switchPage);
    m_nav->setCurrentRow(0);
}

void MainWindow::switchPage(int row)
{
    m_stack->setCurrentIndex(row);
}

// ---------- 患者管理页面 ----------
void MainWindow::setupPatientPage()
{
    auto *page = new QWidget;
    m_patientModel = new QSqlTableModel(this, DatabaseManager::instance().isOpen()
                                         ? QSqlDatabase::database("main_conn")
                                         : QSqlDatabase());
    m_patientModel->setTable("patients");
    m_patientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_patientModel->setHeaderData(1, Qt::Horizontal, "姓名");
    m_patientModel->setHeaderData(2, Qt::Horizontal, "性别");
    m_patientModel->setHeaderData(3, Qt::Horizontal, "年龄");
    m_patientModel->setHeaderData(4, Qt::Horizontal, "电话");
    m_patientModel->setHeaderData(5, Qt::Horizontal, "科室");
    m_patientModel->setHeaderData(6, Qt::Horizontal, "主治医生");
    m_patientModel->setHeaderData(7, Qt::Horizontal, "诊断");
    m_patientModel->setHeaderData(8, Qt::Horizontal, "就诊日期");
    m_patientModel->select();

    m_patientView = new QTableView(page);
    m_patientView->setModel(m_patientModel);
    m_patientView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_patientView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_patientView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_patientView->verticalHeader()->setVisible(false);
    m_patientView->horizontalHeader()->setStretchLastSection(true);
    m_patientView->setColumnHidden(0, true);   // 隐藏主键 id

    // 工具栏
    auto *toolbar = new QToolBar(page);
    toolbar->setMovable(false);
    m_searchEdit = new QLineEdit(page);
    m_searchEdit->setPlaceholderText("按姓名 / 科室查询");
    m_searchEdit->setFixedWidth(180);

    auto *searchBtn = new QPushButton("查询", page);
    auto *addBtn = new QPushButton("新增", page);
    auto *editBtn = new QPushButton("修改", page);
    auto *delBtn = new QPushButton("删除", page);
    auto *refreshBtn = new QPushButton("刷新", page);

    toolbar->addWidget(new QLabel("查询：", page));
    toolbar->addWidget(m_searchEdit);
    toolbar->addWidget(searchBtn);
    toolbar->addSeparator();
    toolbar->addWidget(addBtn);
    toolbar->addWidget(editBtn);
    toolbar->addWidget(delBtn);
    toolbar->addSeparator();
    toolbar->addWidget(refreshBtn);

    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(toolbar);
    lay->addWidget(m_patientView);
    m_stack->addWidget(page);

    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchPatient);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::searchPatient);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addPatient);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editPatient);
    connect(delBtn, &QPushButton::clicked, this, &MainWindow::deletePatient);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshPatient);
    connect(m_patientView, &QTableView::doubleClicked, this, &MainWindow::editPatient);
}

void MainWindow::addPatient()
{
    PatientDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    QSqlRecord rec = m_patientModel->record();
    rec.setValue("name", dlg.name());
    rec.setValue("gender", dlg.gender());
    rec.setValue("age", dlg.age());
    rec.setValue("phone", dlg.phone());
    rec.setValue("department", dlg.department());
    rec.setValue("doctor", dlg.doctor());
    rec.setValue("diagnosis", dlg.diagnosis());
    rec.setValue("visit_date", dlg.visitDate());

    if (!m_patientModel->insertRecord(-1, rec)) {
        QMessageBox::warning(this, "错误", "新增失败：" + m_patientModel->lastError().text());
        return;
    }
    if (!m_patientModel->submitAll()) {
        QMessageBox::warning(this, "错误", "保存失败：" + m_patientModel->lastError().text());
        m_patientModel->revertAll();
        return;
    }
    m_patientModel->select();
    log("新增患者", dlg.name());
    statusBar()->showMessage("已新增患者：" + dlg.name(), 3000);
}

void MainWindow::editPatient()
{
    int row = m_patientView->currentIndex().row();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择一条记录！");
        return;
    }
    QSqlRecord rec = m_patientModel->record(row);

    PatientDialog dlg(this);
    dlg.setName(rec.value("name").toString());
    dlg.setGender(rec.value("gender").toString());
    dlg.setAge(rec.value("age").toInt());
    dlg.setPhone(rec.value("phone").toString());
    dlg.setDepartment(rec.value("department").toString());
    dlg.setDoctor(rec.value("doctor").toString());
    dlg.setDiagnosis(rec.value("diagnosis").toString());
    dlg.setVisitDate(rec.value("visit_date").toString());

    if (dlg.exec() != QDialog::Accepted)
        return;

    rec.setValue("name", dlg.name());
    rec.setValue("gender", dlg.gender());
    rec.setValue("age", dlg.age());
    rec.setValue("phone", dlg.phone());
    rec.setValue("department", dlg.department());
    rec.setValue("doctor", dlg.doctor());
    rec.setValue("diagnosis", dlg.diagnosis());
    rec.setValue("visit_date", dlg.visitDate());

    if (!m_patientModel->setRecord(row, rec)) {
        QMessageBox::warning(this, "错误", "修改失败：" + m_patientModel->lastError().text());
        return;
    }
    if (!m_patientModel->submitAll()) {
        QMessageBox::warning(this, "错误", "保存失败：" + m_patientModel->lastError().text());
        m_patientModel->revertAll();
        return;
    }
    m_patientModel->select();
    log("修改患者", dlg.name());
    statusBar()->showMessage("已修改患者：" + dlg.name(), 3000);
}

void MainWindow::deletePatient()
{
    int row = m_patientView->currentIndex().row();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择一条记录！");
        return;
    }
    QString name = m_patientModel->record(row).value("name").toString();

    // 删除前确认
    if (QMessageBox::question(this, "删除确认",
                              QString("确定要删除患者「%1」吗？").arg(name))
            != QMessageBox::Yes) {
        return;
    }

    m_patientModel->removeRow(row);
    if (!m_patientModel->submitAll()) {
        QMessageBox::warning(this, "错误", "删除失败：" + m_patientModel->lastError().text());
        m_patientModel->revertAll();
        return;
    }
    m_patientModel->select();
    log("删除患者", name);
    statusBar()->showMessage("已删除患者：" + name, 3000);
}

void MainWindow::searchPatient()
{
    QString kw = m_searchEdit->text().trimmed();
    if (kw.isEmpty()) {
        m_patientModel->setFilter("");
    } else {
        // 通用查询：按姓名或科室模糊匹配
        m_patientModel->setFilter(
            QString("name LIKE '%%1%' OR department LIKE '%%1%'").arg(kw));
    }
    m_patientModel->select();
}

void MainWindow::refreshPatient()
{
    m_patientModel->select();
    statusBar()->showMessage("已刷新患者列表", 3000);
}

// ---------- 医生管理页面 ----------
void MainWindow::setupDoctorPage()
{
    auto *page = new QWidget;
    m_doctorModel = new QSqlTableModel(this, QSqlDatabase::database("main_conn"));
    m_doctorModel->setTable("doctors");
    m_doctorModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_doctorModel->setHeaderData(1, Qt::Horizontal, "姓名");
    m_doctorModel->setHeaderData(2, Qt::Horizontal, "科室");
    m_doctorModel->setHeaderData(3, Qt::Horizontal, "职称");
    m_doctorModel->select();

    m_doctorView = new QTableView(page);
    m_doctorView->setModel(m_doctorModel);
    m_doctorView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_doctorView->setColumnHidden(0, true);
    m_doctorView->horizontalHeader()->setStretchLastSection(true);

    auto *toolbar = new QToolBar(page);
    toolbar->setMovable(false);
    auto *addBtn = new QPushButton("新增行", page);
    auto *delBtn = new QPushButton("删除行", page);
    toolbar->addWidget(addBtn);
    toolbar->addWidget(delBtn);

    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(toolbar);
    lay->addWidget(m_doctorView);
    m_stack->addWidget(page);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addDoctor);
    connect(delBtn, &QPushButton::clicked, this, &MainWindow::deleteDoctor);
}

void MainWindow::addDoctor()
{
    int row = m_doctorModel->rowCount();
    m_doctorModel->insertRow(row);
    m_doctorModel->setData(m_doctorModel->index(row, 1), "新医生");
    m_doctorModel->submitAll();
    m_doctorModel->select();
    log("新增医生", "新医生");
}

void MainWindow::deleteDoctor()
{
    int row = m_doctorView->currentIndex().row();
    if (row < 0)
        return;
    m_doctorModel->removeRow(row);
    m_doctorModel->submitAll();
    m_doctorModel->select();
    log("删除医生", "");
}

// ---------- 科室管理页面 ----------
void MainWindow::setupDepartmentPage()
{
    auto *page = new QWidget;
    m_deptModel = new QSqlTableModel(this, QSqlDatabase::database("main_conn"));
    m_deptModel->setTable("departments");
    m_deptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_deptModel->setHeaderData(1, Qt::Horizontal, "科室名称");
    m_deptModel->setHeaderData(2, Qt::Horizontal, "位置");
    m_deptModel->select();

    m_deptView = new QTableView(page);
    m_deptView->setModel(m_deptModel);
    m_deptView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_deptView->setColumnHidden(0, true);
    m_deptView->horizontalHeader()->setStretchLastSection(true);

    auto *toolbar = new QToolBar(page);
    toolbar->setMovable(false);
    auto *addBtn = new QPushButton("新增行", page);
    auto *delBtn = new QPushButton("删除行", page);
    toolbar->addWidget(addBtn);
    toolbar->addWidget(delBtn);

    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(toolbar);
    lay->addWidget(m_deptView);
    m_stack->addWidget(page);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addDepartment);
    connect(delBtn, &QPushButton::clicked, this, &MainWindow::deleteDepartment);
}

void MainWindow::addDepartment()
{
    int row = m_deptModel->rowCount();
    m_deptModel->insertRow(row);
    m_deptModel->setData(m_deptModel->index(row, 1), "新科室");
    m_deptModel->submitAll();
    m_deptModel->select();
    log("新增科室", "新科室");
}

void MainWindow::deleteDepartment()
{
    int row = m_deptView->currentIndex().row();
    if (row < 0)
        return;
    m_deptModel->removeRow(row);
    m_deptModel->submitAll();
    m_deptModel->select();
    log("删除科室", "");
}

// ---------- 操作日志页面 ----------
void MainWindow::setupLogPage()
{
    auto *page = new QWidget;
    m_logModel = new QSqlQueryModel(this);
    m_logModel->setQuery("SELECT operator, action, detail, op_time FROM op_logs ORDER BY log_id DESC",
                         QSqlDatabase::database("main_conn"));
    m_logModel->setHeaderData(0, Qt::Horizontal, "操作者");
    m_logModel->setHeaderData(1, Qt::Horizontal, "操作");
    m_logModel->setHeaderData(2, Qt::Horizontal, "详情");
    m_logModel->setHeaderData(3, Qt::Horizontal, "时间");

    m_logView = new QTableView(page);
    m_logView->setModel(m_logModel);
    m_logView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_logView->horizontalHeader()->setStretchLastSection(true);

    auto *toolbar = new QToolBar(page);
    toolbar->setMovable(false);
    auto *refreshBtn = new QPushButton("刷新", page);
    toolbar->addWidget(refreshBtn);

    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(toolbar);
    lay->addWidget(m_logView);
    m_stack->addWidget(page);

    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshLog);
}

void MainWindow::refreshLog()
{
    m_logModel->setQuery("SELECT operator, action, detail, op_time FROM op_logs ORDER BY log_id DESC",
                         QSqlDatabase::database("main_conn"));
}

void MainWindow::log(const QString &action, const QString &detail)
{
    QSqlQuery q(QSqlDatabase::database("main_conn"));
    q.prepare("INSERT INTO op_logs(operator, action, detail, op_time) VALUES(?, ?, ?, ?)");
    q.addBindValue(m_user);
    q.addBindValue(action);
    q.addBindValue(detail);
    q.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    q.exec();
    refreshLog();
}
