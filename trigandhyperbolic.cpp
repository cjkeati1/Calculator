#include "trigandhyperbolic.h"

#include <QLocale>
#include <cmath>

#include "QtMath"

TrigAndHyperbolic::TrigAndHyperbolic(){

};

QString TrigAndHyperbolic::TrigAndHyperbFunctions(double displayVal,
                                                  QString buttonVal,
                                                  bool* isRadiansMode,
                                                  bool* pressedSecond) {
  const QLocale& cLocale = QLocale::system();
  double result;
  bool isSin =
      ((QString::compare(buttonVal, "sin", Qt::CaseInsensitive) == 0) ||
       (QString::compare(buttonVal, "asin", Qt::CaseInsensitive) == 0));
  bool isCos =
      ((QString::compare(buttonVal, "cos", Qt::CaseInsensitive) == 0) ||
       (QString::compare(buttonVal, "acos", Qt::CaseInsensitive) == 0));
  bool isTan =
      ((QString::compare(buttonVal, "tan", Qt::CaseInsensitive) == 0) ||
       (QString::compare(buttonVal, "atan", Qt::CaseInsensitive) == 0));

  bool isSinh =
      ((QString::compare(buttonVal, "sinh", Qt::CaseInsensitive) == 0) ||
       (QString::compare(buttonVal, "asinh", Qt::CaseInsensitive) == 0));
  bool isCosh =
      ((QString::compare(buttonVal, "cosh", Qt::CaseInsensitive) == 0) ||
       (QString::compare(buttonVal, "acosh", Qt::CaseInsensitive) == 0));
  bool isTanh =
      ((QString::compare(buttonVal, "tanh", Qt::CaseInsensitive) == 0) ||
       (QString::compare(buttonVal, "atanh", Qt::CaseInsensitive) == 0));

  if ((displayVal > 1 || displayVal < -1) &&
          (buttonVal == "asin" || buttonVal == "acos") ||
      (buttonVal == "atanh" && (displayVal >= 1 || displayVal <= -1))) {
    return "Error";
  }

  if (isSin) {
    result =
        (*isRadiansMode)
            ? (!(*pressedSecond)) ? (qSin(displayVal)) : (qAsin(displayVal))
            : ((!(*pressedSecond)) ? qSin(qDegreesToRadians(displayVal))
                                   : qRadiansToDegrees(qAsin(displayVal)));
  } else if (isCos) {
    result =
        (*isRadiansMode)
            ? (!(*pressedSecond)) ? (qCos(displayVal)) : (qAcos(displayVal))
            : ((!(*pressedSecond)) ? qCos(qDegreesToRadians(displayVal))
                                   : qRadiansToDegrees(qAcos(displayVal)));
  } else if (isTan) {
    result =
        (*isRadiansMode)
            ? (!(*pressedSecond)) ? (qTan(displayVal)) : (qAtan(displayVal))
            : ((!(*pressedSecond)) ? qTan(qDegreesToRadians(displayVal))
                                   : qRadiansToDegrees(qAtan(displayVal)));
  }

  // Not periodic functions so rad vs. deg doesn't matter
  else if (isSinh) {
    result = (!(*pressedSecond)) ? sinh(displayVal) : asinh(displayVal);
  } else if (isCosh) {
    result = (!(*pressedSecond)) ? (cosh(displayVal)) : acosh(displayVal);
  } else if (isTanh) {
    result = (!(*pressedSecond)) ? (tanh(displayVal)) : atanh(displayVal);
  }

  if (cLocale.toString(result) == "nan") {
    return "Error";
  } else
    return cLocale.toString(result);
}
