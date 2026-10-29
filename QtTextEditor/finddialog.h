#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

    QString targetText() const;
    bool isDownDirection() const;
    bool isCaseSensitive() const;

signals:
    void findNext(const QString &target, bool down, bool caseSensitive);

private slots:
    void onFindNextClicked();

private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
