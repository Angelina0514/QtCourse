#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FindDialog;
class ReplaceDialog;
class AboutDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 文件菜单 Action 槽
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();

    // 编辑菜单 Action 槽
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionSelectAll_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();

    // 格式菜单 Action 槽
    void on_actionAutoWrap_triggered(bool checked);
    void on_actionShowLineNumber_triggered(bool checked);
    void on_actionFont_triggered();
    void on_actionFontColor_triggered();
    void on_actionFontBgColor_triggered();
    void on_actionEditorBgColor_triggered();

    // 查看菜单 Action 槽
    void on_actionShowToolBar_triggered(bool checked);
    void on_actionShowStatusBar_triggered(bool checked);

    // 帮助菜单 Action 槽
    void on_actionAbout_triggered();

    // 查找与替换对话框信号槽
    void doFindNext(const QString &target, bool down, bool caseSensitive);
    void doReplaceOne(const QString &target, const QString &replacement, bool down, bool caseSensitive);
    void doReplaceAll(const QString &target, const QString &replacement, bool caseSensitive);

    // 状态与界面刷新槽
    void updateStatusBarInfo();
    void updateWindowTitle();

private:
    Ui::MainWindow *ui;

    // 对话框指针
    FindDialog *m_findDialog;
    ReplaceDialog *m_replaceDialog;
    AboutDialog *m_aboutDialog;

    // 当前打开的文件信息
    QString m_currentFileName;
    bool m_isUntitled;

    // 状态栏标签
    QLabel *m_statusLengthLabel;
    QLabel *m_statusLinesLabel;
    QLabel *m_statusLnColLabel;
    QLabel *m_statusAuthorLabel;

    // 核心辅助方法
    void initStatusBar();
    void initConnections();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    bool loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
};

#endif // MAINWINDOW_H
