#include "patientdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>

PatientDialog::PatientDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("患者信息");
    setMinimumWidth(340);

    m_name = new QLineEdit(this);
    m_gender = new QComboBox(this);
    m_gender->addItems(QStringList() << "男" << "女");
    m_age = new QSpinBox(this);
    m_age->setRange(0, 150);
    m_phone = new QLineEdit(this);
    m_department = new QLineEdit(this);
    m_doctor = new QLineEdit(this);
    m_diagnosis = new QLineEdit(this);
    m_visitDate = new QLineEdit(QDate::currentDate().toString("yyyy-MM-dd"), this);

    auto *form = new QFormLayout;
    form->addRow("姓名 *：", m_name);
    form->addRow("性别：", m_gender);
    form->addRow("年龄：", m_age);
    form->addRow("电话：", m_phone);
    form->addRow("科室：", m_department);
    form->addRow("主治医生：", m_doctor);
    form->addRow("诊断：", m_diagnosis);
    form->addRow("就诊日期：", m_visitDate);

    auto *okBtn = new QPushButton("确定", this);
    auto *cancelBtn = new QPushButton("取消", this);
    auto *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    btnRow->addWidget(okBtn);
    btnRow->addWidget(cancelBtn);

    auto *lay = new QVBoxLayout(this);
    lay->addLayout(form);
    lay->addLayout(btnRow);

    connect(okBtn, &QPushButton::clicked, this, &PatientDialog::onOk);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void PatientDialog::onOk()
{
    // 必填字段校验
    if (m_name->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "提示", "姓名为必填项，请填写！");
        m_name->setFocus();
        return;
    }
    accept();
}

void PatientDialog::setName(const QString &v)          { m_name->setText(v); }
QString PatientDialog::name() const                    { return m_name->text().trimmed(); }
void PatientDialog::setGender(const QString &v)        { m_gender->setCurrentText(v); }
QString PatientDialog::gender() const                  { return m_gender->currentText(); }
void PatientDialog::setAge(int v)                      { m_age->setValue(v); }
int PatientDialog::age() const                         { return m_age->value(); }
void PatientDialog::setPhone(const QString &v)         { m_phone->setText(v); }
QString PatientDialog::phone() const                   { return m_phone->text().trimmed(); }
void PatientDialog::setDepartment(const QString &v)     { m_department->setText(v); }
QString PatientDialog::department() const              { return m_department->text().trimmed(); }
void PatientDialog::setDoctor(const QString &v)        { m_doctor->setText(v); }
QString PatientDialog::doctor() const                  { return m_doctor->text().trimmed(); }
void PatientDialog::setDiagnosis(const QString &v)     { m_diagnosis->setText(v); }
QString PatientDialog::diagnosis() const               { return m_diagnosis->text().trimmed(); }
void PatientDialog::setVisitDate(const QString &v)     { m_visitDate->setText(v); }
QString PatientDialog::visitDate() const               { return m_visitDate->text().trimmed(); }
