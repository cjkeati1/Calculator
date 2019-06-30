#include "trigandhyperbolic.h"

#include <QLocale>

#include "QtMath"

TrigAndHyperbolic::TrigAndHyperbolic(){

};

QString TrigAndHyperbolic::TrigAndHyperbFunctions(double displayVal,
                                                  QString buttonVal,
                                                  bool isRadiansMode) {
  const QLocale& cLocale = QLocale::system();
  double result;

  if (QString::compare(buttonVal, "sin", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? qSin(displayVal)
                             : qSin(qDegreesToRadians(displayVal));
  } else if (QString::compare(buttonVal, "cos", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? qCos(displayVal)
                             : qCos(qDegreesToRadians(displayVal));
  } else if (QString::compare(buttonVal, "tan", Qt::CaseInsensitive) == 0) {
    result = (isRadiansMode) ? qTan(displayVal)
                             : qTan(qDegreesToRadians(displayVal));
  }
  // Not periodic functions so rad vs. deg doesn't matter
  else if (QString::compare(buttonVal, "sinh", Qt::CaseInsensitive) == 0) {
    result = sinh(displayVal);
  } else if (QString::compare(buttonVal, "cosh", Qt::CaseInsensitive) == 0) {
    result = cosh(displayVal);
  } else if (QString::compare(buttonVal, "tanh", Qt::CaseInsensitive) == 0) {
    result = tanh(displayVal);
  }

  return cLocale.toString(result);
}
