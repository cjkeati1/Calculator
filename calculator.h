#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

namespace Ui {
class Calculator;
}

class Calculator : public QMainWindow {
  Q_OBJECT

 public:
  explicit Calculator(QWidget* parent = nullptr);
  ~Calculator();
  Ui::Calculator* ui;

 private:
  QString memory = "0";  // Used to store whatever value is inside the display
                         // when user wants to add memory (M+)

  double calcVal = 0.;  // Used to store the number inside the display before a
                        // math operator is used
  double baseX = 0.;
  double powerX = 0.;
  bool justPressedOperator = false;
  int operationsInARowCount = 0;
  int operationsBeforePressingEqualCount = 0;
  bool divTrigger = false;
  bool multTrigger = false;
  bool addTrigger = false;
  bool subTrigger = false;
  bool isWhole = true;
  bool canReplaceCurrentDisplayNum = false;
  bool canChangeOperator = false;
  bool isEnteringPowerY = false;
  bool isEnteringBaseY = false;
  bool isNegativeZero = false;
  bool isRadiansMode = false;

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
  void PressedOnEqualButtonDirectly();
  void PercentButton();
  void Factorial();
  void Squared();
  void Cubed();
  void XToThePowerOfY();
  void EToThePowerOfX();
  void ToTheXPower();
  void SquareRoot();
  void CubeRoot();
  void releaseButtons();
  void YRootX();
  void OneOverX();
  void RadOrDeg();
  void TrigAndHyperbFunctions();
  void NaturalLog();
  void Log();

  bool displayIsEmpty();

  QString deleteCommas();
};

#endif  // CALCULATOR_H
