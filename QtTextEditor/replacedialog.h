#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

signals:
    void findNext(const QString &target, bool down, bool caseSensitive);
    void replaceOne(const QString &target, const QString &replacement, bool down, bool caseSensitive);
    void replaceAll(const QString &target, const QString &replacement, bool caseSensitive);

private slots:
    void onFindNextClicked();
    void onReplaceClicked();
    void onReplaceAllClicked();

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H
