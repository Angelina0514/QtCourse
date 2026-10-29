/********************************************************************************
** Form generated from reading UI file 'replacedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPLACEDIALOG_H
#define UI_REPLACEDIALOG_H

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

class Ui_ReplaceDialog
{
public:
    QLabel *label;
    QLineEdit *editFind;
    QLabel *label_2;
    QLineEdit *editReplace;
    QPushButton *btnFindNext;
    QPushButton *btnReplace;
    QPushButton *btnReplaceAll;
    QPushButton *btnCancel;
    QCheckBox *cbCase;
    QGroupBox *groupBox;
    QRadioButton *rbUp;
    QRadioButton *rbDown;

    void setupUi(QDialog *ReplaceDialog)
    {
        if (ReplaceDialog->objectName().isEmpty())
            ReplaceDialog->setObjectName(QString::fromUtf8("ReplaceDialog"));
        ReplaceDialog->resize(420, 180);
        label = new QLabel(ReplaceDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(15, 20, 75, 25));
        editFind = new QLineEdit(ReplaceDialog);
        editFind->setObjectName(QString::fromUtf8("editFind"));
        editFind->setGeometry(QRect(95, 20, 195, 25));
        label_2 = new QLabel(ReplaceDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(15, 55, 75, 25));
        editReplace = new QLineEdit(ReplaceDialog);
        editReplace->setObjectName(QString::fromUtf8("editReplace"));
        editReplace->setGeometry(QRect(95, 55, 195, 25));
        btnFindNext = new QPushButton(ReplaceDialog);
        btnFindNext->setObjectName(QString::fromUtf8("btnFindNext"));
        btnFindNext->setGeometry(QRect(305, 15, 100, 28));
        btnReplace = new QPushButton(ReplaceDialog);
        btnReplace->setObjectName(QString::fromUtf8("btnReplace"));
        btnReplace->setGeometry(QRect(305, 48, 100, 28));
        btnReplaceAll = new QPushButton(ReplaceDialog);
        btnReplaceAll->setObjectName(QString::fromUtf8("btnReplaceAll"));
        btnReplaceAll->setGeometry(QRect(305, 81, 100, 28));
        btnCancel = new QPushButton(ReplaceDialog);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(305, 114, 100, 28));
        cbCase = new QCheckBox(ReplaceDialog);
        cbCase->setObjectName(QString::fromUtf8("cbCase"));
        cbCase->setGeometry(QRect(15, 115, 95, 20));
        groupBox = new QGroupBox(ReplaceDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(135, 95, 155, 60));
        rbUp = new QRadioButton(groupBox);
        rbUp->setObjectName(QString::fromUtf8("rbUp"));
        rbUp->setGeometry(QRect(10, 25, 65, 20));
        rbDown = new QRadioButton(groupBox);
        rbDown->setObjectName(QString::fromUtf8("rbDown"));
        rbDown->setGeometry(QRect(85, 25, 65, 20));
        rbDown->setChecked(true);

        retranslateUi(ReplaceDialog);

        btnFindNext->setDefault(true);


        QMetaObject::connectSlotsByName(ReplaceDialog);
    } // setupUi

    void retranslateUi(QDialog *ReplaceDialog)
    {
        ReplaceDialog->setWindowTitle(QApplication::translate("ReplaceDialog", "\346\233\277\346\215\242", nullptr));
        label->setText(QApplication::translate("ReplaceDialog", "\346\237\245\346\211\276\347\233\256\346\240\207:", nullptr));
        label_2->setText(QApplication::translate("ReplaceDialog", "\346\233\277\346\215\242\344\270\272:", nullptr));
        btnFindNext->setText(QApplication::translate("ReplaceDialog", "\346\237\245\346\211\276\344\270\213\344\270\200\344\270\252(F)", nullptr));
        btnReplace->setText(QApplication::translate("ReplaceDialog", "\346\233\277\346\215\242(R)", nullptr));
        btnReplaceAll->setText(QApplication::translate("ReplaceDialog", "\345\205\250\351\203\250\346\233\277\346\215\242(A)", nullptr));
        btnCancel->setText(QApplication::translate("ReplaceDialog", "\345\217\226\346\266\210", nullptr));
        cbCase->setText(QApplication::translate("ReplaceDialog", "\345\214\272\345\210\206\345\244\247\345\260\217\345\206\231", nullptr));
        groupBox->setTitle(QApplication::translate("ReplaceDialog", "\346\226\271\345\220\221", nullptr));
        rbUp->setText(QApplication::translate("ReplaceDialog", "\345\220\221\344\270\212(U)", nullptr));
        rbDown->setText(QApplication::translate("ReplaceDialog", "\345\220\221\344\270\213(D)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReplaceDialog: public Ui_ReplaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPLACEDIALOG_H
