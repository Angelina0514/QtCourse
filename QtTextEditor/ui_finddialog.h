/********************************************************************************
** Form generated from reading UI file 'finddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_FindDialog
{
public:
    QLabel *label;
    QLineEdit *editTarget;
    QPushButton *btnFindNext;
    QPushButton *btnCancel;
    QCheckBox *cbCase;
    QGroupBox *groupBox;
    QRadioButton *rbUp;
    QRadioButton *rbDown;

    void setupUi(QDialog *FindDialog)
    {
        if (FindDialog->objectName().isEmpty())
            FindDialog->setObjectName(QString::fromUtf8("FindDialog"));
        FindDialog->resize(400, 160);
        label = new QLabel(FindDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(15, 20, 75, 25));
        editTarget = new QLineEdit(FindDialog);
        editTarget->setObjectName(QString::fromUtf8("editTarget"));
        editTarget->setGeometry(QRect(95, 20, 175, 25));
        btnFindNext = new QPushButton(FindDialog);
        btnFindNext->setObjectName(QString::fromUtf8("btnFindNext"));
        btnFindNext->setGeometry(QRect(285, 18, 100, 28));
        btnCancel = new QPushButton(FindDialog);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(285, 60, 100, 28));
        cbCase = new QCheckBox(FindDialog);
        cbCase->setObjectName(QString::fromUtf8("cbCase"));
        cbCase->setGeometry(QRect(15, 85, 95, 20));
        groupBox = new QGroupBox(FindDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(120, 60, 150, 60));
        rbUp = new QRadioButton(groupBox);
        rbUp->setObjectName(QString::fromUtf8("rbUp"));
        rbUp->setGeometry(QRect(10, 25, 65, 20));
        rbDown = new QRadioButton(groupBox);
        rbDown->setObjectName(QString::fromUtf8("rbDown"));
        rbDown->setGeometry(QRect(80, 25, 65, 20));
        rbDown->setChecked(true);

        retranslateUi(FindDialog);

        btnFindNext->setDefault(true);


        QMetaObject::connectSlotsByName(FindDialog);
    } // setupUi

    void retranslateUi(QDialog *FindDialog)
    {
        FindDialog->setWindowTitle(QApplication::translate("FindDialog", "\346\237\245\346\211\276", nullptr));
        label->setText(QApplication::translate("FindDialog", "\346\237\245\346\211\276\347\233\256\346\240\207:", nullptr));
        btnFindNext->setText(QApplication::translate("FindDialog", "\346\237\245\346\211\276\344\270\213\344\270\200\344\270\252(F)", nullptr));
        btnCancel->setText(QApplication::translate("FindDialog", "\345\217\226\346\266\210", nullptr));
        cbCase->setText(QApplication::translate("FindDialog", "\345\214\272\345\210\206\345\244\247\345\260\217\345\206\231", nullptr));
        groupBox->setTitle(QApplication::translate("FindDialog", "\346\226\271\345\220\221", nullptr));
        rbUp->setText(QApplication::translate("FindDialog", "\345\220\221\344\270\212(U)", nullptr));
        rbDown->setText(QApplication::translate("FindDialog", "\345\220\221\344\270\213(D)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindDialog: public Ui_FindDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H
