#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

namespace Ui {
class Calculator;
}

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    const double PI = 3.141592653589793; // Constant for pi
    const double EULER = 2.718281828459045; // Constant for Euler's number
    double calcVal = 0.; // Used to store the number inside the display before a math operator is used
    double memory = 0.; // Used to store whatever value is inside the display when user wants to add memory (M+)
    bool divTrigger = false;
    bool multTrigger = false;
    bool addTrigger = false;
    bool subTrigger = false;
    bool decimalTrigger = false;
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;

private slots:
    void NumPressed();
    void MathButtonPressed();
    void EqualButton();
    void ChangeNumberSign();
    void ClearButtonPressed();
    void MemoryAdd();
    void MemoryClear();
    void MemoryGet();
    void PiPressed();
    void EulersNumberPressed();
    void RandomNumberPressed();
    void DecimalPointPressed();
    void ClearAllTriggers();
    void ClearOperatorTriggers();
    void GetMathButton();
};

#endif // CALCULATOR_H
