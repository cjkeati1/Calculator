#include "calculator.h"

#include <QLocale>
#include <QtMath>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

#include "math.h"
#include "random"
#include "time.h"
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
  connect(ui->Button_Squared, SIGNAL(released()), this, SLOT(Squared()));
  connect(ui->Button_Cubed, SIGNAL(released()), this, SLOT(Cubed()));
  connect(ui->Button_EulerPower, SIGNAL(released()), this,
          SLOT(EToThePowerOfX()));
  connect(ui->Button_TenPower, SIGNAL(released()), this, SLOT(TenToTheX()));
  connect(ui->Button_XPowerY, SIGNAL(released()), this, SLOT(XToThePowerOfY()));
  connect(ui->Button_SquareRoot, SIGNAL(released()), this, SLOT(SquareRoot()));
  connect(ui->Button_CubeRoot, SIGNAL(released()), this, SLOT(CubeRoot()));
  connect(ui->Button_OneOverX, SIGNAL(released()), this, SLOT(OneOverX()));
  connect(ui->Button_YRootX, SIGNAL(released()), this, SLOT(YRootX()));
  connect(ui->Button_Rad, SIGNAL(released()), this, SLOT(RadOrDeg()));
  connect(ui->Button_Sin, SIGNAL(released()), this, SLOT(TrigFunctions()));
  connect(ui->Button_Cos, SIGNAL(released()), this, SLOT(TrigFunctions()));
  connect(ui->Button_Tan, SIGNAL(released()), this, SLOT(TrigFunctions()));
  connect(ui->Button_Sinh, SIGNAL(released()), this, SLOT(TrigFunctions()));
  connect(ui->Button_Cosh, SIGNAL(released()), this, SLOT(TrigFunctions()));
  connect(ui->Button_Tanh, SIGNAL(released()), this, SLOT(TrigFunctions()));
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
  }

  // Adding zeroes onto existing number after decimal place (converting to
  // double after this would yield the same number as before) Example:
  // static_cast<double>(25.560) == 25.56 So treat it as a string
  else if (butVal == "0" && !isWhole) {
    QString newVal = displayVal + butVal;
    ui->Display->setText(newVal);
  } else {
    QString newVal = displayVal + butVal;
    double dblNewVal = newVal.toDouble();
    ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    // ui->Display->setText(cLocale.toString(dblNewVal));
    // ui->Display->setText(QString::number(dblNewVal));
  }
  justPressedOperator = false;
  canReplaceCurrentDisplayNum = false;
  ;
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

  if (!isEnteringPowerY && !isEnteringBaseY) {
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
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();

  ui->Display->setText(memory);

  isWhole = (dblDisplayVal - static_cast<int>(dblDisplayVal) == 0);
  canReplaceCurrentDisplayNum = true;
  releaseButtons();
}

void Calculator::PiPressed() {
  ui->Display->setText(QString::number(PI));
  isWhole = false;
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
  releaseButtons();
}

void Calculator::EulersNumberPressed() {
  ui->Display->setText(QString::number(EULER));
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
  isEnteringPowerY = false;
  baseX = 0.;
  ui->Button_Divide->setDown(divTrigger);
  ui->Button_Multiply->setDown(multTrigger);
  ui->Button_Add->setDown(addTrigger);
  ui->Button_Subtract->setDown(subTrigger);
  ui->Button_XPowerY->setDown(isEnteringPowerY);
}

void Calculator::ClearOperatorTriggers() {
  divTrigger = false;
  multTrigger = false;
  addTrigger = false;
  subTrigger = false;
  isEnteringPowerY = false;
  ui->Button_Divide->setDown(divTrigger);
  ui->Button_Multiply->setDown(multTrigger);
  ui->Button_Add->setDown(addTrigger);
  ui->Button_Subtract->setDown(subTrigger);
  ui->Button_XPowerY->setDown(isEnteringPowerY);
}

void Calculator::GetMathButton() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  QPushButton* button = static_cast<QPushButton*>(sender());
  QString butVal = button->text();

  if (operationsInARowCount > 1)
    ClearOperatorTriggers();  // If we're changing operator from one to another,
                              // clear triggers
  else
    calcVal =
        displayVal.toDouble();  // If we're not changing, thereby meaning the
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
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();
  double percentVal = dblDisplayVal * .01;
  QString resultString = cLocale.toString(percentVal);
  ui->Display->setText(resultString);
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::Factorial() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  isWhole = (dblDisplayVal - static_cast<int>(dblDisplayVal) == 0);

  bool isZero = dblDisplayVal == 0 || dblDisplayVal == 0.0;

  if (!isWhole || dblDisplayVal < 0 && !isZero) {
    ui->Display->setText("Error");

  } else if (!isZero) {
    unsigned long long int factorial = 1;
    while (dblDisplayVal > 0) {
      factorial *= dblDisplayVal;

      if (factorial == 0) {  // Overflow Occurred
        ui->Display->setText("Error");
        canReplaceCurrentDisplayNum = true;
        justPressedOperator = false;

        return;
      }
      dblDisplayVal--;
    }
    QString resultString = cLocale.toString(factorial);
    ui->Display->setText(resultString);
  } else {  // If user is finding the factorial of 0
    ui->Display->setText("1");
  }
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::Squared() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  bool wasZero = dblDisplayVal == 0 || dblDisplayVal == 0.0;

  double square = dblDisplayVal;
  square *= dblDisplayVal;

  if ((square == 0 || square == 0.0) && !wasZero)
    ui->Display->setText("Error");
  else {
    QString resultString = cLocale.toString(square);
    ui->Display->setText(resultString);
  }

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::Cubed() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  bool wasZero = dblDisplayVal == 0 || dblDisplayVal == 0.0;

  double cube = dblDisplayVal;
  cube *= (dblDisplayVal * dblDisplayVal);

  if ((cube == 0 || cube == 0.0) && !wasZero)
    ui->Display->setText("Error");
  else {
    QString resultString = cLocale.toString(cube);
    ui->Display->setText(resultString);
  }
  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::EToThePowerOfX() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  double eulerToThePowerOfX = std::exp(dblDisplayVal);
  QString resultString = cLocale.toString(eulerToThePowerOfX);
  if (resultString == "0")
    ui->Display->setText("Error");
  else
    ui->Display->setText(resultString);

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::TenToTheX() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  double TenToThePowerOfX = pow(10, dblDisplayVal);
  QString resultString = cLocale.toString(TenToThePowerOfX);
  if (resultString == "0")
    ui->Display->setText("Error");
  else
    ui->Display->setText(resultString);

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::XToThePowerOfY() {
  operationsInARowCount++;
  if (operationsInARowCount > 1)
    ClearOperatorTriggers();  // If we're changing operator from one to another,
                              // clear triggers

  if (!isEnteringPowerY) {
    QString displayVal = deleteCommas();
    baseX = displayVal.toDouble();
    canReplaceCurrentDisplayNum = true;
    isEnteringPowerY = true;
  }
  ui->Button_XPowerY->setDown(isEnteringPowerY);
  justPressedOperator = true;
}

void Calculator::YRootX() {
  operationsInARowCount++;
  if (operationsInARowCount > 1)
    ClearOperatorTriggers();  // If we're changing operator from one to another,
                              // clear triggers

  if (!isEnteringBaseY) {
    QString displayVal = deleteCommas();
    powerX = displayVal.toDouble();
    canReplaceCurrentDisplayNum = true;
    isEnteringBaseY = true;
  }
  ui->Button_YRootX->setDown(isEnteringBaseY);
  justPressedOperator = true;
}

void Calculator::SquareRoot() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  if (dblDisplayVal < 0) {
    ui->Display->setText("Error");
  } else {
    double squareRoot = sqrt(dblDisplayVal);

    QString resultString = cLocale.toString(squareRoot);
    ui->Display->setText(resultString);
  }

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

void Calculator::CubeRoot() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();

  double squareRoot = cbrt(dblDisplayVal);

  QString resultString = cLocale.toString(squareRoot);
  ui->Display->setText(resultString);

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
  // ui->Button_Rad->setDown(isRadiansMode);
  isRadiansMode = !isRadiansMode;

  ui->Button_Rad->setText((isRadiansMode) ? "Deg" : "Rad");

  ui->Button_Rad->setChecked(isRadiansMode);

  // Temp
}
void Calculator::TrigFunctions() {
  QString displayVal = deleteCommas();
  double dblDisplayVal = displayVal.toDouble();
  const QLocale& cLocale = QLocale::system();
  QPushButton* button = static_cast<QPushButton*>(sender());
  QString butVal = button->text();
  double result;

  if (QString::compare(butVal, "sin", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? qSin(dblDisplayVal)
                             : qSin(qDegreesToRadians(dblDisplayVal));
  } else if (QString::compare(butVal, "cos", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? qCos(dblDisplayVal)
                             : qCos(qDegreesToRadians(dblDisplayVal));
  } else if (QString::compare(butVal, "tan", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? qTan(dblDisplayVal)
                             : qTan(qDegreesToRadians(dblDisplayVal));
  } else if (QString::compare(butVal, "sinh", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? sinh(dblDisplayVal)
                             : sinh(qDegreesToRadians(dblDisplayVal));
  } else if (QString::compare(butVal, "cosh", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? cosh(dblDisplayVal)
                             : cosh(qDegreesToRadians(dblDisplayVal));
  } else if (QString::compare(butVal, "tanh", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? tanh(dblDisplayVal)
                             : tanh(qDegreesToRadians(dblDisplayVal));
  }

  QString resultString = cLocale.toString(result);
  ui->Display->setText(resultString);

  canReplaceCurrentDisplayNum = true;
  justPressedOperator = false;
}

QString Calculator::deleteCommas() {
  QString displayVal = ui->Display->text();
  displayVal = displayVal.replace(",", "");

  return displayVal;
}
