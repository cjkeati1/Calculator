#include "calculator.h"

#include <QLocale>
#include <QtMath>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

#include "factorial.h"
#include "math.h"
#include "random"
#include "time.h"
#include "trigandhyperbolic.h"
#include "ui_calculator.h"

Calculator::Calculator(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::Calculator) {
  ui->setupUi(this);
  ui->Display->setText(QString::number(calcVal));

  srand((unsigned)time(nullptr));  // Randomizes seed by returning the integer
                                   // number of seconds from the system clock

  QPushButton* numButtons[10];
  for (int i = 0; i < 10; i++) {
    QString butName = "Button_" + QString::number(i);
    numButtons[i] = Calculator::findChild<QPushButton*>(butName);
    connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
  }
  // ui->Button_SquareRoot->setText(QStringLiteral("\u221A"));
  //  ui->Button_Squared->setText("x<sup>2</sup>");
  // ui->Button_CubeRoot->setText(QStringLiteral("\u221B"));

  isSecondMode = &pressedSecond;
  isRad_ptr = &isRadiansMode;

  // Connecting buttons to their respective methods
  connect(ui->Button_Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
  connect(ui->Button_Subtract, SIGNAL(released()), this,
          SLOT(MathButtonPressed()));
  connect(ui->Button_Multiply, SIGNAL(released()), this,
          SLOT(MathButtonPressed()));
  connect(ui->Button_Divide, SIGNAL(released()), this,
          SLOT(MathButtonPressed()));
  connect(ui->Button_Equals, SIGNAL(released()), this, SLOT(EqualButton()));
  connect(ui->Button_Sign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
  connect(ui->Button_Clear, SIGNAL(released()), this,
          SLOT(ClearButtonPressed()));
  connect(ui->Button_Equals, SIGNAL(released()), this,
          SLOT(PressedOnEqualButtonDirectly()));
  connect(ui->Button_MemClear, SIGNAL(released()), this, SLOT(MemoryClear()));
  connect(ui->Button_MemAdd, SIGNAL(released()), this, SLOT(MemoryAdd()));
  connect(ui->Button_MemGet, SIGNAL(released()), this, SLOT(MemoryGet()));
  connect(ui->Button_Pi, SIGNAL(released()), this, SLOT(PiPressed()));
  connect(ui->Button_Euler, SIGNAL(released()), this,
          SLOT(EulersNumberPressed()));
  connect(ui->Button_Rand, SIGNAL(released()), this,
          SLOT(RandomNumberPressed()));
  connect(ui->Button_DecimalPoint, SIGNAL(released()), this,
          SLOT(DecimalPointPressed()));
  connect(ui->Button_Percent, SIGNAL(released()), this, SLOT(PercentButton()));
  connect(ui->Button_Factorial, SIGNAL(released()), this, SLOT(Factorial()));
  connect(ui->Button_Squared, SIGNAL(released()), this, SLOT(SquaredOrCubed()));
  connect(ui->Button_Cubed, SIGNAL(released()), this, SLOT(SquaredOrCubed()));
  connect(ui->Button_EulerPower, SIGNAL(released()), this,
          SLOT(EToThePowerOfX()));
  connect(ui->Button_TenPower, SIGNAL(released()), this, SLOT(ToTheXPower()));
  connect(ui->Button_TwoPower, SIGNAL(released()), this, SLOT(ToTheXPower()));
  connect(ui->Button_XPowerY, SIGNAL(released()), this, SLOT(Powers()));
  connect(ui->Button_SquareRoot, SIGNAL(released()), this,
          SLOT(SquareOrCubeRoot()));
  connect(ui->Button_CubeRoot, SIGNAL(released()), this,
          SLOT(SquareOrCubeRoot()));
  connect(ui->Button_OneOverX, SIGNAL(released()), this, SLOT(OneOverX()));
  connect(ui->Button_YRootX, SIGNAL(released()), this, SLOT(Powers()));
  connect(ui->Button_Rad, SIGNAL(released()), this, SLOT(RadOrDeg()));
  connect(ui->Button_Sin, SIGNAL(released()), this,
          SLOT(TrigAndHyperbFunctions()));
  connect(ui->Button_Cos, SIGNAL(released()), this,
          SLOT(TrigAndHyperbFunctions()));
  connect(ui->Button_Tan, SIGNAL(released()), this,
          SLOT(TrigAndHyperbFunctions()));
  connect(ui->Button_Sinh, SIGNAL(released()), this,
          SLOT(TrigAndHyperbFunctions()));
  connect(ui->Button_Cosh, SIGNAL(released()), this,
          SLOT(TrigAndHyperbFunctions()));
  connect(ui->Button_Tanh, SIGNAL(released()), this,
          SLOT(TrigAndHyperbFunctions()));
  connect(ui->Button_Ln, SIGNAL(released()), this, SLOT(NaturalLog()));
  connect(ui->Button_Log10, SIGNAL(released()), this, SLOT(Log()));
  connect(ui->Button_Log2, SIGNAL(released()), this, SLOT(Log()));
  connect(ui->Button_SciNotation, SIGNAL(released()), this, SLOT(EE()));
  connect(ui->Button_Logy, SIGNAL(released()), this, SLOT(LogBaseY()));
  connect(ui->Button_Second, SIGNAL(released()), this, SLOT(SecondPressed()));
}

Calculator::~Calculator() {
  delete ui;
}

void Calculator::releaseButtons() {
  ui->Button_Divide->setDown(false);
  ui->Button_Multiply->setDown(false);
  ui->Button_Add->setDown(false);
  ui->Button_Subtract->setDown(false);
  ui->Button_XPowerY->setDown(false);
  ui->Button_YRootX->setDown(false);
  ui->Button_SciNotation->setDown(false);
  ui->Button_Logy->setDown(false);
}

void Calculator::NumPressed() {
  const QLocale& cLocale = QLocale::system();
  QPushButton* button = static_cast<QPushButton*>(sender());
  QString butVal = button->text();
  QString displayVal = deleteCommas();
  QRegExp reg("[-]?[0-9.]*");
  isNegativeZero = displayVal == "-0";
  if (((displayIsEmpty() || justPressedOperator) &&
       (isWhole || operationsBeforePressingEqualCount > 1)) ||
      (canReplaceCurrentDisplayNum)) {  // Display is empty and decimal point
                                        // was NOT pressed
    if (isNegativeZero)
      ui->Display->setText("-" + butVal);
    else
      ui->Display->setText(butVal);

    releaseButtons();
  } else if (butVal == "0" &&
             !isWhole) {  // Adding zeroes onto existing number after decimal
                          // place (converting to double after this would yield
                          // the same number as before) Example:
                          // static_cast<double>(25.560) == 25.56 So treat it as
                          // a string
    QString newVal = displayVal + butVal;
    ui->Display->setText(newVal);
  } else {
    QString newVal = displayVal + butVal;
    double dblNewVal = newVal.toDouble();
    ui->Display->setText(QString::number(dblNewVal, 'g', 16));
  }
  justPressedOperator = false;
  canReplaceCurrentDisplayNum = false;
}

void Calculator::MathButtonPressed() {
  operationsInARowCount++;
  isWhole = true;
  QPushButton* button = static_cast<QPushButton*>(sender());
  QString butVal = button->text();

  if (!justPressedOperator) {
    operationsBeforePressingEqualCount++;
  }  // 2 + 2 + (change to -) 2 (press equal) would be 2 operations before
     // pressing equal

  if (operationsBeforePressingEqualCount > 1)
    canChangeOperator = true;

  if (operationsBeforePressingEqualCount > 1 && !justPressedOperator) {
    EqualButton();
    GetMathButton();
  } else {
    GetMathButton();
    //  ui->Display->setText(butVal);
  }
  justPressedOperator = true;
}

bool Calculator::displayIsEmpty() {
  QString displayVal = deleteCommas();
  return ((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)) &&
         isWhole;
}

void Calculator::EqualButton() {
  const QLocale& cLocale = QLocale::system();
  QString displayVal = deleteCommas();

  if (!isEnteringPowerY && !isEnteringBaseY && !isEnteringEE &&
      !isEnteringLogBaseY) {
    operationsInARowCount = 0;
    if (operationsBeforePressingEqualCount < 2)
      operationsBeforePressingEqualCount = 0;
    double solution = 0.0;
    double dblDisplayVal = displayVal.toDouble();

    if (addTrigger || subTrigger || multTrigger || divTrigger) {
      if (addTrigger) {
        solution = calcVal + dblDisplayVal;
      } else if (subTrigger) {
        solution = calcVal - dblDisplayVal;
      } else if (multTrigger) {
        solution = calcVal * dblDisplayVal;
      } else {
        solution = calcVal / dblDisplayVal;
      }
      QString result = QString::number(solution);
      if (result.contains("inf")) {
        ui->Display->setText("Error");
      } else
        ui->Display->setText(QString::number(solution));
    }
    ClearOperatorTriggers();
    isWhole = (solution - static_cast<int>(solution) == 0);
  } else if (isEnteringPowerY) {
    double powerY = displayVal.toDouble();
    double result = pow(baseX, powerY);

    QString resultString = cLocale.toString(result);
    if (resultString == "0" && baseX != 0.)
      ui->Display->setText("Error");
    else
      ui->Display->setText(resultString);
    isEnteringPowerY = false;
    canReplaceCurrentDisplayNum = true;
    baseX = 0;
  } else if (isEnteringEE) {
    double EE = displayVal.toDouble();
    double result = sciNotationBase * (pow(10, EE));
    QString resultString = cLocale.toString(result);

    ui->Display->setText(resultString);
    isEnteringEE = false;
    canReplaceCurrentDisplayNum = true;
    sciNotationBase = 0;
  } else if (isEnteringLogBaseY) {
    double baseY = displayVal.toDouble();
    double result = (log(logExponentX) / log(baseY));
    QString resultString = cLocale.toString(result);

    ui->Display->setText(resultString);
    isEnteringLogBaseY = false;
    canReplaceCurrentDisplayNum = true;
    baseY = 0;
  } else {
    double baseY = displayVal.toDouble();
    double result = pow(baseY, powerX);
    QString resultString = cLocale.toString(result);

    if (resultString == "0" && baseY != 0.)
      ui->Display->setText("Error");
    else
      ui->Display->setText(resultString);
    isEnteringBaseY = false;
    canReplaceCurrentDisplayNum = true;
    powerX = 0;
  }
  releaseButtons();
}

void Calculator::ChangeNumberSign() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();

  // If display value is 0 or we an operation is triggered (thus, we want to
  // enter a new number)
  if (displayVal == "0" || justPressedOperator)
    ui->Display->setText("-0");
  else if ((dblDisplayVal == 0 ||
            displayVal.at(displayVal.length() - 1) == "0") &&
           !canReplaceCurrentDisplayNum) {  // If display value is 0 or ends
                                            // with a 0 after decimal point,
                                            // concatenate "-"
    if (displayVal.toStdString().find("-") ==
        std::string::npos) {  // If its a positive 0
      QString newNum = "-" + displayVal;
      ui->Display->setText(newNum);
    } else {  // If its a negative 0
      QString newNum = displayVal.replace("-", "");
      ui->Display->setText(newNum);
    }
  } else {  // If display value is a non-zero number
    QRegExp reg("[-]?[0-9.]*");
    QString result;
    if (reg.exactMatch(displayVal)) {
      const QLocale& cLocale = QLocale::system();
      if (displayVal.toStdString().find("-") ==
          std::string::npos) {  // If its a positive 0
        result = "-" + displayVal;
      } else {
        result = displayVal.replace("-", "");
      }
      ui->Display->setText(result);
    }
  }
  releaseButtons();
}

void Calculator::ClearButtonPressed() {
  ui->Display->setText("0");
  ClearAllTriggers();
  operationsBeforePressingEqualCount = 0;
}

void Calculator::MemoryAdd() {
  QString displayVal = ui->Display->text();
  memory = displayVal;
}

void Calculator::MemoryClear() {
  memory = "0";
}

void Calculator::MemoryGet() {
  double displayVal = deleteCommas().toDouble();

  ui->Display->setText(memory);

  isWhole = (displayVal - static_cast<int>(displayVal) == 0);
  canReplaceCurrentDisplayNum = true;
  releaseButtons();
}

void Calculator::PiPressed() {
  ui->Display->setText(QString::number(M_PI));
  isWhole = false;
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
  releaseButtons();
}

void Calculator::EulersNumberPressed() {
  ui->Display->setText(QString::number(M_E));
  isWhole = false;
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
  releaseButtons();
}

void Calculator::RandomNumberPressed() {
  double max = 1.;
  double min = 0.;

  // Generates random number from [min, max)
  double r = min + (rand() / (RAND_MAX / (max - min)));

  ui->Display->setText(QString::number(r));

  justPressedOperator = false;
  canReplaceCurrentDisplayNum = true;
  releaseButtons();
}

void Calculator::DecimalPointPressed() {
  QString butVal = ".";
  QString displayVal = ui->Display->text();
  double dblDisplayVal = displayVal.toDouble();

  // Check to see if it contains a "." and it minus its integer value equals 0
  isWhole = (dblDisplayVal - static_cast<int>(dblDisplayVal) == 0) &&
            (displayVal.toStdString().find(".") == std::string::npos);

  isNegativeZero = displayVal == "-0";

  if ((displayIsEmpty() || justPressedOperator || canReplaceCurrentDisplayNum))
    ui->Display->setText((isNegativeZero) ? "-0." : "0.");
  else if (isWhole) {
    QString newVal = displayVal + butVal;
    ui->Display->setText(newVal);
  }
  justPressedOperator = false;
  canReplaceCurrentDisplayNum = false;
  isWhole = false;
  releaseButtons();
}

void Calculator::ClearAllTriggers() {
  divTrigger = false;
  multTrigger = false;
  addTrigger = false;
  subTrigger = false;
  isWhole = true;
  isEnteringPowerY = false;
  justPressedOperator = false;
  canReplaceCurrentDisplayNum = false;
  isEnteringBaseY = false;
  baseX = 0.;

  releaseButtons();
}

void Calculator::ClearOperatorTriggers() {
  divTrigger = false;
  multTrigger = false;
  addTrigger = false;
  subTrigger = false;
  isEnteringPowerY = false;
  isEnteringBaseY = false;
  ui->Button_Divide->setDown(divTrigger);
  ui->Button_Multiply->setDown(multTrigger);
  ui->Button_Add->setDown(addTrigger);
  ui->Button_Subtract->setDown(subTrigger);
  ui->Button_XPowerY->setDown(isEnteringPowerY);
  ui->Button_YRootX->setDown(isEnteringBaseY);
}

void Calculator::GetMathButton() {
  double displayVal = deleteCommas().toDouble();
  const QLocale& cLocale = QLocale::system();

  QPushButton* button = static_cast<QPushButton*>(sender());
  QString butVal = button->text();

  if (operationsInARowCount > 1)
    ClearOperatorTriggers();  // If we're changing operator from one to another,
                              // clear triggers
  else
    calcVal = displayVal;  // If we're not changing, thereby meaning the
                           // display value is a number and not an operator
                           // symbol, then set calcVal equal to that

  if (QString::compare(butVal, "/", Qt::CaseInsensitive) ==
      0) {  // Set the respective trigger to the selected operator to true
    divTrigger = true;
  } else if (QString::compare(butVal, "*", Qt::CaseInsensitive) == 0) {
    multTrigger = true;
  } else if (QString::compare(butVal, "+", Qt::CaseInsensitive) == 0) {
    addTrigger = true;
  } else {
    subTrigger = true;
  }

  ui->Button_Divide->setDown(divTrigger);
  ui->Button_Multiply->setDown(multTrigger);
  ui->Button_Add->setDown(addTrigger);
  ui->Button_Subtract->setDown(subTrigger);
}

void Calculator::PressedOnEqualButtonDirectly() {
  operationsBeforePressingEqualCount = 0;
  canReplaceCurrentDisplayNum = true;
  operationsInARowCount = 0;
}

void Calculator::PercentButton() {
  double displayVal = deleteCommas().toDouble();
  const QLocale& cLocale = QLocale::system();
  double percentVal = displayVal * .01;
  QString resultString = cLocale.toString(percentVal);
  ui->Display->setText(resultString);
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::Factorial() {
  if (deleteCommas() == "Error")
    return;

  double displayVal = deleteCommas().toDouble();
  const QLocale& cLocale = QLocale::system();

  class Factorial fact;

  QString result = fact.getFactorial(displayVal);

  if (result == "Error")
    ui->Display->setText("Error");
  else {
    ui->Display->setText(result);
  }

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::SquaredOrCubed() {
  double displayVal = deleteCommas().toDouble();

  const QLocale& cLocale = QLocale::system();
  QString butVal = static_cast<QPushButton*>(sender())->text();

  if (deleteCommas() == "Error")
    return;

  bool wasZero = displayVal == 0 || displayVal == 0.0;

  double result = displayVal;
  result *= (butVal == "x^2") ? displayVal : (displayVal * displayVal);

  if ((result == 0 || result == 0.0) && !wasZero)
    ui->Display->setText("Error");
  else {
    QString resultString = cLocale.toString(result);
    if (resultString == "inf")
      ui->Display->setText("Error");
    else
      ui->Display->setText(resultString);
  }

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::EToThePowerOfX() {
  double displayVal = deleteCommas().toDouble();
  const QLocale& cLocale = QLocale::system();

  if (deleteCommas() == "Error")
    return;

  double eulerToThePowerOfX = std::exp(displayVal);
  QString resultString = cLocale.toString(eulerToThePowerOfX);
  if (resultString == "0" || resultString == "inf")
    ui->Display->setText("Error");
  else
    ui->Display->setText(resultString);

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::ToTheXPower() {
  double displayVal = deleteCommas().toDouble();
  const QLocale& cLocale = QLocale::system();
  QPushButton* button = static_cast<QPushButton*>(sender());
  QString butVal = button->text();

  if (deleteCommas() == "Error")
    return;

  double ToTheXPower = pow((butVal == "10^x") ? 10 : 2, displayVal);
  QString resultString = cLocale.toString(ToTheXPower);
  if (resultString == "0" || resultString == "inf")
    ui->Display->setText("Error");
  else
    ui->Display->setText(resultString);

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::Powers() {
  QString butVal = static_cast<QPushButton*>(sender())->text();
  operationsInARowCount++;
  if (operationsInARowCount > 1)
    ClearOperatorTriggers();  // If we're changing operator from one to another,
                              // clear triggers

  if (!isEnteringPowerY && butVal == "x^y") {
    if (deleteCommas() == "Error")
      return;
    baseX = deleteCommas().toDouble();
    canReplaceCurrentDisplayNum = true;
    isEnteringPowerY = true;
    ui->Button_XPowerY->setDown(isEnteringPowerY);
  } else if (!isEnteringBaseY && butVal == "y^x") {
    if (deleteCommas() == "Error")
      return;
    powerX = deleteCommas().toDouble();
    canReplaceCurrentDisplayNum = true;
    isEnteringBaseY = true;
    ui->Button_YRootX->setDown(isEnteringBaseY);
  }

  justPressedOperator = true;
}

void Calculator::SquareOrCubeRoot() {
  double displayVal = deleteCommas().toDouble();

  const QLocale& cLocale = QLocale::system();
  QString butVal = static_cast<QPushButton*>(sender())->text();

  if (deleteCommas() == "Error")
    return;

  if (displayVal < 0 &&
      butVal == "sqrt(x)") {  // Can't square root a negative number
    ui->Display->setText("Error");
  } else {
    double squareRoot =
        (butVal == "sqrt(x)") ? sqrt(displayVal) : cbrt(displayVal);
    //    std::stringstream ss;
    //    ss.imbue(std::locale(""));

    //    ss << std::setprecision(16)<< std::fixed << squareRoot;
    //    std::string s = ss.str();
    //    ui->Display->setText(QString::fromStdString(s));

    QString resultString = cLocale.toString(squareRoot);
    ui->Display->setText(resultString);
  }

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::OneOverX() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  if (dblDisplayVal == 0)
    ui->Display->setText("Error");
  else {
    double oneOverX = 1 / dblDisplayVal;

    QString resultString = cLocale.toString(oneOverX);
    ui->Display->setText(resultString);
  }
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::RadOrDeg() {
  isRadiansMode = !isRadiansMode;
  ui->Button_Rad->setText((isRadiansMode) ? "Deg" : "Rad");
}

void Calculator::TrigAndHyperbFunctions() {
  double displayVal = deleteCommas().toDouble();

  QString butVal = static_cast<QPushButton*>(sender())->text();
  QString result;

  TrigAndHyperbolic trigFunctions;

  if (deleteCommas() == "Error") {
    // canReplaceCurrentDisplayNum = true;
    return;
  }

  result = trigFunctions.TrigAndHyperbFunctions(displayVal, butVal, isRad_ptr,
                                                isSecondMode);

  ui->Display->setText(result);

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::NaturalLog() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  if (dblDisplayVal == 0)
    ui->Display->setText("Error");
  else {
    double naturalLog = qLn(dblDisplayVal);

    QString resultString = cLocale.toString(naturalLog);
    if (resultString == "nan")
      ui->Display->setText("Error");
    else
      ui->Display->setText(resultString);
  }
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::Log() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();
  QPushButton* button = static_cast<QPushButton*>(sender());
  QString butVal = button->text();
  double result;

  if (dblDisplayVal == 0)
    ui->Display->setText("Error");
  else {
    double naturalLog =
        (butVal == "log10") ? log10(dblDisplayVal) : log2(dblDisplayVal);

    QString resultString = cLocale.toString(naturalLog);
    if (resultString == "nan")
      ui->Display->setText("Error");
    else
      ui->Display->setText(resultString);
  }
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}
void Calculator::LogBaseY() {
  operationsInARowCount++;
  if (operationsInARowCount > 1)
    ClearOperatorTriggers();  // If we're changing operator from one to another,
                              // clear triggers

  if (!isEnteringLogBaseY) {
    if (deleteCommas() == "Error")
      return;
    logExponentX = deleteCommas().toDouble();
    canReplaceCurrentDisplayNum = true;
    isEnteringLogBaseY = true;
    ui->Button_Logy->setDown(isEnteringLogBaseY);
  }

  justPressedOperator = true;
}

void Calculator::EE() {
  operationsInARowCount++;
  if (operationsInARowCount > 1)
    ClearOperatorTriggers();  // If we're changing operator from one to another,
                              // clear triggers

  if (!isEnteringEE) {
    if (deleteCommas() == "Error")
      return;

    sciNotationBase = deleteCommas().toDouble();
    canReplaceCurrentDisplayNum = true;
    isEnteringEE = true;
  }
  ui->Button_SciNotation->setDown(isEnteringEE);
  justPressedOperator = true;
}
void Calculator::SecondPressed() {
  pressedSecond = !pressedSecond;
  ui->Button_Second->setText((pressedSecond) ? "1st" : "2nd");
  ui->Button_Sin->setText((pressedSecond) ? "asin" : "sin");
  ui->Button_Cos->setText((pressedSecond) ? "acos" : "cos");
  ui->Button_Tan->setText((pressedSecond) ? "atan" : "tan");
  ui->Button_Sinh->setText((pressedSecond) ? "asinh" : "sinh");
  ui->Button_Cosh->setText((pressedSecond) ? "acosh" : "cosh");
  ui->Button_Tanh->setText((pressedSecond) ? "atanh" : "tanh");
}

QString Calculator::deleteCommas() {
  QString displayVal = ui->Display->text();
  displayVal = displayVal.replace(",", "");

  return displayVal;
}
