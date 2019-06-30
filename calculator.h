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

  bool divTrigger = false;
  bool multTrigger = false;
  bool addTrigger = false;
  bool subTrigger = false;

 protected:
  bool canChangeOperator = false;
  bool isEnteringPowerY = false;
  bool isEnteringBaseY = false;
  bool isNegativeZero = false;
  bool isRadiansMode = false;
  bool isWhole = true;
  bool justPressedOperator = false;
  double baseX = 0.;
  double powerX = 0.;
  double calcVal = 0.;
  bool canReplaceCurrentDisplayNum = false;
  int operationsInARowCount = 0;
  int operationsBeforePressingEqualCount = 0;
  QString deleteCommas();

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
  void SquaredOrCubed();
  void XToThePowerOfY();
  void EToThePowerOfX();
  void ToTheXPower();
  void SquareOrCubeRoot();
  void releaseButtons();
  void YRootX();
  void OneOverX();
  void RadOrDeg();
  void TrigAndHyperbFunctions();
  void NaturalLog();
  void Log();

  bool displayIsEmpty();


};

#endif  // CALCULATOR_H
