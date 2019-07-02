#ifndef TRIGANDHYPERBOLIC_H
#define TRIGANDHYPERBOLIC_H

#include <QMainWindow>


class TrigAndHyperbolic
{
public:
    TrigAndHyperbolic();

    QString TrigAndHyperbFunctions(double displayNum, QString buttonVal, bool *isRadiansMode, bool *pressedSecond);
};

#endif // TRIGANDHYPERBOLIC_H
