/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QLabel *labelTitle;
    QLabel *labelIcon;
    QLabel *labelDev;
    QLabel *labelId;
    QLabel *labelEmail;
    QLabel *labelVersion;
    QPushButton *btnOk;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(380, 280);
        labelTitle = new QLabel(AboutDialog);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        labelTitle->setGeometry(QRect(30, 20, 320, 45));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignCenter);
        labelIcon = new QLabel(AboutDialog);
        labelIcon->setObjectName(QString::fromUtf8("labelIcon"));
        labelIcon->setGeometry(QRect(30, 80, 90, 100));
        QFont font1;
        font1.setPointSize(48);
        labelIcon->setFont(font1);
        labelIcon->setAlignment(Qt::AlignCenter);
        labelDev = new QLabel(AboutDialog);
        labelDev->setObjectName(QString::fromUtf8("labelDev"));
        labelDev->setGeometry(QRect(140, 85, 220, 22));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font2.setPointSize(11);
        labelDev->setFont(font2);
        labelId = new QLabel(AboutDialog);
        labelId->setObjectName(QString::fromUtf8("labelId"));
        labelId->setGeometry(QRect(140, 115, 220, 22));
        labelId->setFont(font2);
        labelEmail = new QLabel(AboutDialog);
        labelEmail->setObjectName(QString::fromUtf8("labelEmail"));
        labelEmail->setGeometry(QRect(140, 145, 220, 22));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font3.setPointSize(10);
        labelEmail->setFont(font3);
        labelVersion = new QLabel(AboutDialog);
        labelVersion->setObjectName(QString::fromUtf8("labelVersion"));
        labelVersion->setGeometry(QRect(35, 215, 200, 25));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Consolas"));
        font4.setPointSize(10);
        labelVersion->setFont(font4);
        btnOk = new QPushButton(AboutDialog);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setGeometry(QRect(260, 215, 85, 30));

        retranslateUi(AboutDialog);

        btnOk->setDefault(true);


        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "\345\205\263\344\272\216", nullptr));
        labelTitle->setText(QApplication::translate("AboutDialog", "\345\260\217\347\211\233\346\226\207\346\234\254\347\274\226\350\276\221", nullptr));
        labelIcon->setText(QApplication::translate("AboutDialog", "\360\237\223\235", nullptr));
        labelDev->setText(QApplication::translate("AboutDialog", "\345\274\200\345\217\221\350\200\205\357\274\232\344\275\231\346\230\212\345\223\262", nullptr));
        labelId->setText(QApplication::translate("AboutDialog", "\345\255\246\345\217\267\357\274\2322024414290339", nullptr));
        labelEmail->setText(QApplication::translate("AboutDialog", "\346\214\207\345\257\274\350\200\201\345\270\210\357\274\232\345\210\230\346\226\207\346\236\234", nullptr));
        labelVersion->setText(QApplication::translate("AboutDialog", "V1.0 build 202610", nullptr));
        btnOk->setText(QApplication::translate("AboutDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
