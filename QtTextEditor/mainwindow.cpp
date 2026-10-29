#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finddialog.h"
#include "replacedialog.h"
#include "aboutdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QColorDialog>
#include <QCloseEvent>
#include <QStyle>
#include <QTextCursor>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_findDialog(nullptr)
    , m_replaceDialog(nullptr)
    , m_aboutDialog(nullptr)
    , m_currentFileName("")
    , m_isUntitled(true)
{
    ui->setupUi(this);

    // 设置基础属性
    setWindowTitle("新建文本文件 - 编辑器");
    resize(850, 600);

    // 设置图标
    ui->actionNew->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->actionSaveAs->setIcon(style()->standardIcon(QStyle::SP_DriveHDIcon));
    ui->actionUndo->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    ui->actionRedo->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    ui->actionAbout->setIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));

    // 初始化状态栏
    initStatusBar();

    // 初始化信号与槽
    initConnections();

    // 创建子对话框
    m_findDialog = new FindDialog(this);
    connect(m_findDialog, &FindDialog::findNext, this, &MainWindow::doFindNext);

    m_replaceDialog = new ReplaceDialog(this);
    connect(m_replaceDialog, &ReplaceDialog::findNext, this, &MainWindow::doFindNext);
    connect(m_replaceDialog, &ReplaceDialog::replaceOne, this, &MainWindow::doReplaceOne);
    connect(m_replaceDialog, &ReplaceDialog::replaceAll, this, &MainWindow::doReplaceAll);

    m_aboutDialog = new AboutDialog(this);

    // 初始剪贴使能状态
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    setCurrentFile("");
    updateStatusBarInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initStatusBar()
{
    m_statusLengthLabel = new QLabel("length: 0", this);
    m_statusLinesLabel  = new QLabel("lines: 1", this);
    m_statusLnColLabel  = new QLabel("Ln: 1  Col: 1", this);
    m_statusAuthorLabel = new QLabel("余昊哲", this);

    m_statusLengthLabel->setMinimumWidth(80);
    m_statusLinesLabel->setMinimumWidth(80);
    m_statusLnColLabel->setMinimumWidth(120);
    m_statusAuthorLabel->setMinimumWidth(70);

    ui->statusbar->addPermanentWidget(m_statusLengthLabel);
    ui->statusbar->addPermanentWidget(m_statusLinesLabel);
    ui->statusbar->addPermanentWidget(m_statusLnColLabel);
    ui->statusbar->addPermanentWidget(m_statusAuthorLabel);
}

void MainWindow::initConnections()
{
    // 文本与光标变化触发状态栏刷新与修改标记
    connect(ui->editor, &CodeEditor::textChanged, this, &MainWindow::updateStatusBarInfo);
    connect(ui->editor, &CodeEditor::cursorPositionChanged, this, &MainWindow::updateStatusBarInfo);
    connect(ui->editor->document(), &QTextDocument::modificationChanged, this, &MainWindow::updateWindowTitle);

    // 复制、剪切、撤销、重做的使能控制
    connect(ui->editor, &CodeEditor::copyAvailable, ui->actionCut, &QAction::setEnabled);
    connect(ui->editor, &CodeEditor::copyAvailable, ui->actionCopy, &QAction::setEnabled);
    connect(ui->editor, &CodeEditor::undoAvailable, ui->actionUndo, &QAction::setEnabled);
    connect(ui->editor, &CodeEditor::redoAvailable, ui->actionRedo, &QAction::setEnabled);
}

void MainWindow::updateStatusBarInfo()
{
    // 计算文本总字符数与总行数
    int length = ui->editor->toPlainText().length();
    int lines = ui->editor->blockCount();

    // 计算当前光标所在行列号
    QTextCursor cursor = ui->editor->textCursor();
    int lineNum = cursor.blockNumber() + 1;
    int colNum = cursor.columnNumber() + 1;

    m_statusLengthLabel->setText(QString("length: %1").arg(length));
    m_statusLinesLabel->setText(QString("lines: %1").arg(lines));
    m_statusLnColLabel->setText(QString("Ln: %1  Col: %2").arg(lineNum).arg(colNum));
}

void MainWindow::updateWindowTitle()
{
    QString shownName = m_isUntitled ? "新建文本文件" : QFileInfo(m_currentFileName).fileName();
    if (ui->editor->document()->isModified()) {
        setWindowTitle("*" + shownName + " - 编辑器");
    } else {
        setWindowTitle(shownName + " - 编辑器");
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_currentFileName = fileName;
    m_isUntitled = fileName.isEmpty();
    ui->editor->document()->setModified(false);
    updateWindowTitle();
}

bool MainWindow::maybeSave()
{
    if (!ui->editor->document()->isModified())
        return true;

    QString shownName = m_isUntitled ? "新建文本文件" : QFileInfo(m_currentFileName).fileName();
    QMessageBox::StandardButton ret = QMessageBox::warning(
        this, "警告",
        QString("文件 \"%1\" 已被修改，是否保存？").arg(shownName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (ret == QMessageBox::Save) {
        on_actionSave_triggered();
        return !ui->editor->document()->isModified();
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "错误", QString("无法写入文件 %1:\n%2").arg(fileName, file.errorString()));
        return false;
    }

    QTextStream out(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    out.setCodec("UTF-8");
#endif
    out << ui->editor->toPlainText();

    setCurrentFile(fileName);
    ui->statusbar->showMessage("文件保存成功", 3000);
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "错误", QString("无法读取文件 %1:\n%2").arg(fileName, file.errorString()));
        return false;
    }

    QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    in.setCodec("UTF-8");
#endif
    ui->editor->setPlainText(in.readAll());

    setCurrentFile(fileName);
    ui->statusbar->showMessage("文件读取成功", 3000);
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

// ==================== 文件操作 ====================

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave()) {
        ui->editor->clear();
        setCurrentFile("");
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(
            this, "打开文件", "", "文本文件 (*.txt);;C++源码 (*.cpp *.h);;所有文件 (*.*)");
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_isUntitled) {
        on_actionSaveAs_triggered();
    } else {
        saveFile(m_currentFileName);
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "另存为", "", "文本文件 (*.txt);;所有文件 (*.*)");
    if (!fileName.isEmpty()) {
        saveFile(fileName);
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

// ==================== 编辑操作 ====================

void MainWindow::on_actionUndo_triggered()
{
    ui->editor->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->editor->redo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->editor->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->editor->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->editor->paste();
}

void MainWindow::on_actionSelectAll_triggered()
{
    ui->editor->selectAll();
}

void MainWindow::on_actionFind_triggered()
{
    m_findDialog->show();
    m_findDialog->raise();
    m_findDialog->activateWindow();
}

void MainWindow::on_actionReplace_triggered()
{
    m_replaceDialog->show();
    m_replaceDialog->raise();
    m_replaceDialog->activateWindow();
}

// ==================== 格式操作 ====================

void MainWindow::on_actionAutoWrap_triggered(bool checked)
{
    ui->editor->setLineWrapMode(checked ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
}

void MainWindow::on_actionShowLineNumber_triggered(bool checked)
{
    ui->editor->setLineNumbersVisible(checked);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, ui->editor->font(), this, "选择字体");
    if (ok) {
        ui->editor->setFont(font);
    }
}

void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择字体颜色");
    if (color.isValid()) {
        QPalette p = ui->editor->palette();
        p.setColor(QPalette::Text, color);
        ui->editor->setPalette(p);
    }
}

void MainWindow::on_actionFontBgColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this, "选择字体背景色");
    if (color.isValid()) {
        QTextCursor cursor = ui->editor->textCursor();
        if (cursor.hasSelection()) {
            QTextCharFormat fmt;
            fmt.setBackground(color);
            cursor.mergeCharFormat(fmt);
        } else {
            QMessageBox::information(this, "提示", "请先选中文本，然后再设置字体背景色。");
        }
    }
}

void MainWindow::on_actionEditorBgColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择编辑器背景色");
    if (color.isValid()) {
        QPalette p = ui->editor->palette();
        p.setColor(QPalette::Base, color);
        ui->editor->setPalette(p);
    }
}

// ==================== 查看操作 ====================

void MainWindow::on_actionShowToolBar_triggered(bool checked)
{
    ui->mainToolBar->setVisible(checked);
}

void MainWindow::on_actionShowStatusBar_triggered(bool checked)
{
    ui->statusbar->setVisible(checked);
}

// ==================== 帮助操作 ====================

void MainWindow::on_actionAbout_triggered()
{
    m_aboutDialog->exec();
}

// ==================== 查找与替换核心逻辑 ====================

void MainWindow::doFindNext(const QString &target, bool down, bool caseSensitive)
{
    QTextDocument::FindFlags flags;
    if (!down) {
        flags |= QTextDocument::FindBackward;
    }
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    bool found = ui->editor->find(target, flags);
    if (!found) {
        // 如果未找到，提示用户
        QMessageBox::information(this, "查找结果", QString("未找到 \"%1\"。").arg(target));
    }
}

void MainWindow::doReplaceOne(const QString &target, const QString &replacement, bool down, bool caseSensitive)
{
    QTextCursor cursor = ui->editor->textCursor();

    Qt::CaseSensitivity cs = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    // 如果当前选中的文本刚好等于查找目标，则直接替换并查找下一个
    if (cursor.hasSelection() && cursor.selectedText().compare(target, cs) == 0) {
        cursor.insertText(replacement);
        doFindNext(target, down, caseSensitive);
    } else {
        // 否则先找下一个
        doFindNext(target, down, caseSensitive);
        cursor = ui->editor->textCursor();
        if (cursor.hasSelection() && cursor.selectedText().compare(target, cs) == 0) {
            cursor.insertText(replacement);
        }
    }
}

void MainWindow::doReplaceAll(const QString &target, const QString &replacement, bool caseSensitive)
{
    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    // 从文档开头开始替换
    QTextCursor cursor = ui->editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->editor->setTextCursor(cursor);

    int count = 0;
    while (ui->editor->find(target, flags)) {
        ui->editor->textCursor().insertText(replacement);
        ++count;
    }

    QMessageBox::information(this, "全部替换", QString("全部替换完成，共替换 %1 处。").arg(count));
}
