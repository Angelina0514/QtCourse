#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QPushButton>
#include <cmath>

// ============================================================
// 构造与析构
// ============================================================



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("计算器");
    setFixedSize(320, 500);

    // 初始化所有状态
    resetState();

    // 绑定按钮信号，并为按钮设置类型属性（供 QSS 使用）
    connectButtons();

    // 应用样式表（必须在 setProperty 之后调用，以确保属性已生效）
    applyStyleSheet();

    // 初始显示 "0"
    setDisplay("0");
    setExpression("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================================
// 状态重置
// ============================================================

void MainWindow::resetState()
{
    m_firstOperand      = 0.0;
    m_operator          = "";
    m_isSecondOperand   = false;
    m_hasResult         = false;
    m_waitingForOperand = false;
}

// ============================================================
// 按钮绑定（同时为每个按钮标记 btnType 属性用于 QSS 分类）
// ============================================================

void MainWindow::connectButtons()
{
    // ---- 数字键：统一用 lambda 捕获按钮文本 ----
    QList<QPushButton*> digitBtns = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };
    for (auto *btn : digitBtns) {
        // 设置自定义属性，供 QSS 选择器 QPushButton[btnType="digit"] 匹配
        btn->setProperty("btnType", "digit");
        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            inputDigit(btn->text());
        });
    }

    // ---- 小数点 ----
    ui->btnDecimal->setProperty("btnType", "digit");
    connect(ui->btnDecimal, &QPushButton::clicked, this, [this]() {
        inputDecimal();
    });

    // ---- 操作符键 ----
    QList<QPushButton*> opBtns = {
        ui->btnAdd, ui->btnSubtract, ui->btnMultiply, ui->btnDivide
    };
    for (auto *btn : opBtns) {
        btn->setProperty("btnType", "operator");
        connect(btn, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    }

    // ---- 功能键 ----
    QList<QPushButton*> funcBtns = {
        ui->btnCE, ui->btnC, ui->btnBackspace, ui->btnToggleSign, ui->btnPercent
    };
    for (auto *btn : funcBtns) {
        btn->setProperty("btnType", "func");
    }

    // ---- 等号（蓝色主题） ----
    ui->btnEqual->setProperty("btnType", "equal");

    // ---- 功能键各自的槽 ----
    connect(ui->btnEqual,      &QPushButton::clicked, this, &MainWindow::onEqualClicked);
    connect(ui->btnBackspace,  &QPushButton::clicked, this, &MainWindow::onBackspaceClicked);
    connect(ui->btnCE,         &QPushButton::clicked, this, &MainWindow::onCEClicked);
    connect(ui->btnC,          &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->btnToggleSign, &QPushButton::clicked, this, &MainWindow::onToggleSignClicked);
    connect(ui->btnPercent,    &QPushButton::clicked, this, &MainWindow::onPercentClicked);
}

// ============================================================
// 显示屏辅助
// ============================================================

void MainWindow::setDisplay(const QString &text)
{
    ui->displayLabel->setText(text);
}

void MainWindow::setExpression(const QString &text)
{
    ui->expressionLabel->setText(text);
}

// ============================================================
// 数字格式化（去除多余浮点尾缀）
// ============================================================

QString MainWindow::formatNumber(double value) const
{
    // 无穷大 / NaN 特殊处理
    if (std::isinf(value)) return "溢出";
    if (std::isnan(value)) return "无效运算";

    // 'g' 格式会自动去掉末尾的零
    QString s = QString::number(value, 'g', 15);
    return s;
}

// ============================================================
// 核心：统一数字输入（鼠标 & 键盘共用此函数）
// ============================================================

void MainWindow::inputDigit(const QString &digit)
{
    QString current = ui->displayLabel->text();

    if (m_waitingForOperand) {
        // 已经输入了操作符，下一位数字替换显示屏（不追加）
        setDisplay(digit);
        m_waitingForOperand = false;
    } else if (m_hasResult) {
        // 刚完成一次计算（按过等号），输入数字应开启全新的计算
        resetState();
        setDisplay(digit);
        setExpression("");
    } else {
        // 正常追加数字
        if (current == "0") {
            // 当前显示是 "0" 时直接替换，避免出现 "07"
            setDisplay(digit);
        } else {
            // 限制最多 15 位，防止溢出
            if (current.length() < 15) {
                setDisplay(current + digit);
            }
        }
    }
}

// ============================================================
// 核心：统一小数点输入
// ============================================================

void MainWindow::inputDecimal()
{
    QString current = ui->displayLabel->text();

    if (m_waitingForOperand) {
        // 刚选了操作符，从 "0." 开始
        setDisplay("0.");
        m_waitingForOperand = false;
        return;
    }

    if (m_hasResult) {
        // 刚完成计算，重新开始输入
        resetState();
        setDisplay("0.");
        setExpression("");
        return;
    }

    // 已经有小数点则忽略（防止 "1..2" 这类错误）
    if (!current.contains('.')) {
        setDisplay(current + ".");
    }
}

// ============================================================
// 核心：统一操作符输入
// ============================================================

void MainWindow::inputOperator(const QString &op)
{
    QString current      = ui->displayLabel->text();
    double  currentValue = current.toDouble();

    if (!m_operator.isEmpty() && !m_waitingForOperand) {
        // 连续运算：先算出前一步的结果
        // 例如 "3 + 5 *" → 先算 3+5=8，再以 8 为第一操作数
        bool ok = true;
        double result = calculate(m_firstOperand, currentValue, m_operator, ok);
        if (!ok) {
            setDisplay("错误：除数为零");
            setExpression("");
            resetState();
            return;
        }
        m_firstOperand = result;
        setDisplay(formatNumber(result));
        setExpression(formatNumber(result) + " " + op);
    } else {
        // 首次输入操作符，或 m_hasResult == true 后继续运算
        m_firstOperand = currentValue;
        setExpression(formatNumber(currentValue) + " " + op);
    }

    m_operator          = op;
    m_isSecondOperand   = true;
    m_waitingForOperand = true; // 等待第二个操作数
    m_hasResult         = false;
}

// ============================================================
// 核心：统一等号处理
// ============================================================

void MainWindow::inputEqual()
{
    // 没有操作符时按等号无意义
    if (m_operator.isEmpty()) return;

    QString current      = ui->displayLabel->text();
    double  currentValue = current.toDouble();

    bool   ok     = true;
    double result = calculate(m_firstOperand, currentValue, m_operator, ok);

    if (!ok) {
        // 除数为零：显示错误，重置状态
        setDisplay("错误：除数为零");
        setExpression("");
        resetState();
        return;
    }

    // 在表达式栏显示完整算式（如 "12 + 5 ="）
    setExpression(formatNumber(m_firstOperand) + " " + m_operator + " " +
                  formatNumber(currentValue) + " =");
    setDisplay(formatNumber(result));

    // 标记刚完成计算
    m_hasResult         = true;
    m_waitingForOperand = false;
    m_isSecondOperand   = false;
    m_operator          = "";     // 清空操作符，防止再次按等号重复运算
    m_firstOperand      = result; // 保留结果以备连续运算
}

// ============================================================
// 核心：退格
// ============================================================

void MainWindow::inputBackspace()
{
    // 刚完成计算或正在等待操作数时，退格等同于 CE（清除当前显示）
    if (m_hasResult || m_waitingForOperand) {
        onCEClicked();
        return;
    }

    QString current = ui->displayLabel->text();

    if (current.length() <= 1 || current == "0") {
        // 只有一个字符，或本来就是 "0"，退格后恢复 "0"
        setDisplay("0");
    } else {
        current.chop(1);
        // 退格后若只剩负号，也恢复 "0"
        if (current == "-") {
            setDisplay("0");
        } else {
            setDisplay(current);
        }
    }
}

// ============================================================
// 实际运算
// ============================================================

double MainWindow::calculate(double first, double second,
                              const QString &op, bool &ok)
{
    ok = true;
    if (op == "+") return first + second;
    if (op == "-") return first - second;
    if (op == "*") return first * second;
    if (op == "/") {
        if (qFuzzyIsNull(second)) {
            ok = false; // 除数为零错误
            return 0.0;
        }
        return first / second;
    }
    return 0.0;
}

// ============================================================
// Slots：操作符按钮（将按钮显示文字转为内部操作符字符串）
// ============================================================

void MainWindow::onOperatorClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) return;

    QString text = btn->text();
    QString op;
    if      (text == "÷") op = "/";
    else if (text == "×") op = "*";
    else if (text == "−") op = "-";
    else if (text == "+") op = "+";
    else                  op = text;

    inputOperator(op);
}

void MainWindow::onEqualClicked()
{
    inputEqual();
}

void MainWindow::onBackspaceClicked()
{
    inputBackspace();
}

void MainWindow::onCEClicked()
{
    // CE：只清除当前输入，保留第一个操作数和操作符
    setDisplay("0");
    m_waitingForOperand = false;
    m_hasResult         = false;
}

void MainWindow::onClearClicked()
{
    // C：全部清除，回到初始状态
    resetState();
    setDisplay("0");
    setExpression("");
}

void MainWindow::onToggleSignClicked()
{
    QString current = ui->displayLabel->text();
    if (current == "0" || current.isEmpty()) return;

    double val = current.toDouble();
    val = -val;
    setDisplay(formatNumber(val));
}

void MainWindow::onPercentClicked()
{
    QString current = ui->displayLabel->text();
    double val = current.toDouble();
    val /= 100.0;
    setDisplay(formatNumber(val));
}

// ============================================================
// 键盘事件（鼠标和键盘复用同一套 input* 函数）
// ============================================================

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_0: inputDigit("0"); break;
    case Qt::Key_1: inputDigit("1"); break;
    case Qt::Key_2: inputDigit("2"); break;
    case Qt::Key_3: inputDigit("3"); break;
    case Qt::Key_4: inputDigit("4"); break;
    case Qt::Key_5: inputDigit("5"); break;
    case Qt::Key_6: inputDigit("6"); break;
    case Qt::Key_7: inputDigit("7"); break;
    case Qt::Key_8: inputDigit("8"); break;
    case Qt::Key_9: inputDigit("9"); break;

    case Qt::Key_Period:
    case Qt::Key_Comma:
        inputDecimal();
        break;

    case Qt::Key_Plus:
        inputOperator("+");
        break;
    case Qt::Key_Minus:
        inputOperator("-");
        break;
    case Qt::Key_Asterisk:
        inputOperator("*");
        break;
    case Qt::Key_Slash:
        inputOperator("/");
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        inputEqual();
        break;

    case Qt::Key_Backspace:
        inputBackspace();
        break;

    case Qt::Key_Escape:
        // Escape → 全部清除（等同于按 C）
        onClearClicked();
        break;

    case Qt::Key_Delete:
        // Delete → 清除当前输入（等同于按 CE）
        onCEClicked();
        break;

    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

// ============================================================
// QSS 样式表
// ============================================================

void MainWindow::applyStyleSheet()
{
    // Qt 的属性选择器语法：QPushButton[btnType="digit"]
    // 注意：setProperty 必须在 applyStyleSheet 之前调用，样式才能正确匹配
    setStyleSheet(R"(
        /* ===== 主窗口 & 中央 Widget ===== */
        QMainWindow, QWidget {
            background-color: #f3f3f3;
        }

        /* ===== 显示屏容器 ===== */
        QFrame#displayFrame {
            background-color: #f3f3f3;
            border: none;
        }

        /* ===== 表达式历史行（小字） ===== */
        QLabel#expressionLabel {
            color: #888888;
            font-size: 14px;
            font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
            padding: 0px 14px;
            qproperty-alignment: AlignRight;
        }

        /* ===== 主显示数字（大字） ===== */
        QLabel#displayLabel {
            color: #1a1a1a;
            font-size: 40px;
            font-weight: 300;
            font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
            padding: 0px 14px 8px 14px;
            qproperty-alignment: AlignRight;
        }

        /* ===== 所有按钮基础样式 ===== */
        QPushButton {
            font-size: 18px;
            font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
            border: none;
            border-radius: 4px;
            min-height: 56px;
        }

        /* ===== 数字按钮（白色背景） ===== */
        QPushButton[btnType="digit"] {
            background-color: #ffffff;
            color: #1a1a1a;
        }
        QPushButton[btnType="digit"]:hover {
            background-color: #e8e8e8;
        }
        QPushButton[btnType="digit"]:pressed {
            background-color: #d0d0d0;
        }

        /* ===== 操作符按钮（浅灰背景） ===== */
        QPushButton[btnType="operator"] {
            background-color: #e9e9e9;
            color: #1a1a1a;
        }
        QPushButton[btnType="operator"]:hover {
            background-color: #d6d6d6;
        }
        QPushButton[btnType="operator"]:pressed {
            background-color: #c3c3c3;
        }

        /* ===== 功能按钮（CE / C / ⌫ / +/- / %，浅灰） ===== */
        QPushButton[btnType="func"] {
            background-color: #e9e9e9;
            color: #1a1a1a;
        }
        QPushButton[btnType="func"]:hover {
            background-color: #d6d6d6;
        }
        QPushButton[btnType="func"]:pressed {
            background-color: #c3c3c3;
        }

        /* ===== 等号按钮（Windows 蓝） ===== */
        QPushButton[btnType="equal"] {
            background-color: #0078d7;
            color: #ffffff;
            font-size: 22px;
        }
        QPushButton[btnType="equal"]:hover {
            background-color: #1a87e0;
        }
        QPushButton[btnType="equal"]:pressed {
            background-color: #006cbe;
        }
    )");
}
