#include "factorial.h"

#include "QLocale"

Factorial::Factorial() {}

QString Factorial::getFactorial(double displayVal) {
  const QLocale& cLocale = QLocale::system();
  bool isWhole = (displayVal - static_cast<int>(displayVal) == 0);

  bool isZero = displayVal == 0 || displayVal == 0.0;

  if (!isWhole || displayVal < 0 && !isZero) {
    return "Error";

  } else if (!isZero) {
    unsigned long long int factorial = 1;
    while (displayVal > 0) {
      factorial *= displayVal;

      if (factorial == 0) {  // Overflow Occurred
        return "Error";
      }
      displayVal--;
    }
    return cLocale.toString(factorial);
  } else {
    return "1";  // Factorial of 0
  }
}
