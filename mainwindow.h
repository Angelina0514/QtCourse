#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 拦截键盘事件，支持键盘操作（与鼠标共用同一套 input* 函数）
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // 操作符按钮槽（+、−、×、÷）
    void onOperatorClicked();

    // 等号按钮槽
    void onEqualClicked();

    // 退格按钮槽
    void onBackspaceClicked();

    // CE（清除当前输入）按钮槽
    void onCEClicked();

    // C（全部清除）按钮槽
    void onClearClicked();

    // 正负号切换按钮槽
    void onToggleSignClicked();

    // 百分号按钮槽
    void onPercentClicked();

private:
    Ui::MainWindow *ui;

    // ========== 计算器核心状态 ==========

    // 第一个操作数（在输入操作符后保存）
    double m_firstOperand;

    // 当前操作符（"+", "-", "*", "/"），空字符串表示尚未选择
    QString m_operator;

    // true 表示当前正在输入第二个操作数
    bool m_isSecondOperand;

    // true 表示刚完成一次计算（按下等号后），此时再输入数字开始全新计算
    bool m_hasResult;

    // true 表示刚选了操作符，等待用户输入下一个操作数（显示屏应被替换而非追加）
    bool m_waitingForOperand;

    // ========== 统一输入函数（鼠标 & 键盘共用） ==========

    // 处理数字 0-9 的输入
    void inputDigit(const QString &digit);

    // 处理小数点输入
    void inputDecimal();

    // 处理操作符输入
    void inputOperator(const QString &op);

    // 处理等号
    void inputEqual();

    // 处理退格
    void inputBackspace();

    // ========== 辅助函数 ==========

    // 执行实际运算，返回结果；若除数为零则 ok=false
    double calculate(double first, double second, const QString &op, bool &ok);

    // 将 double 格式化为显示字符串（使用 'g' 格式，自动去除尾零）
    QString formatNumber(double value) const;

    // 更新主显示屏
    void setDisplay(const QString &text);

    // 更新表达式/历史行（小字显示）
    void setExpression(const QString &text);

    // 重置所有状态变量到初始值
    void resetState();

    // 绑定所有按钮的 clicked 信号，并设置 btnType 属性（用于 QSS 分类样式）
    void connectButtons();

    // 应用 QSS 样式表（类 Windows 计算器浅色风格）
    void applyStyleSheet();
};

#endif // MAINWINDOW_H
