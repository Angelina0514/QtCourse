#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QComboBox;
class QSpinBox;

// 患者信息编辑对话框（新增 / 修改共用）
class PatientDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PatientDialog(QWidget *parent = nullptr);

    void setName(const QString &v);
    QString name() const;
    void setGender(const QString &v);
    QString gender() const;
    void setAge(int v);
    int age() const;
    void setPhone(const QString &v);
    QString phone() const;
    void setDepartment(const QString &v);
    QString department() const;
    void setDoctor(const QString &v);
    QString doctor() const;
    void setDiagnosis(const QString &v);
    QString diagnosis() const;
    void setVisitDate(const QString &v);
    QString visitDate() const;

private slots:
    void onOk();

private:
    QLineEdit *m_name;
    QComboBox *m_gender;
    QSpinBox  *m_age;
    QLineEdit *m_phone;
    QLineEdit *m_department;
    QLineEdit *m_doctor;
    QLineEdit *m_diagnosis;
    QLineEdit *m_visitDate;
};

#endif // PATIENTDIALOG_H
