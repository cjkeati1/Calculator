#include "calculator.h"
#include "ui_calculator.h"
#include "math.h"
#include "random"
#include "time.h"

#include <cstdlib>
#include <ctime>

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->Display->setText(QString::number(calcVal));

    srand((unsigned)time(NULL)); // Randomizes seed by returning the integer number of seconds from the system clock

    QPushButton *numButtons[10];
    for (int i = 0; i < 10; i++) {
        QString butName = "Button_" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    // Connecting buttons to their respective methods
    connect(ui->Button_Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Button_Subtract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Button_Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Button_Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Button_Equals, SIGNAL(released()), this, SLOT(EqualButton()));
    connect(ui->Button_Sign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
    connect(ui->Button_Clear, SIGNAL(released()), this, SLOT(ClearButtonPressed()));
    connect(ui->Button_Equals, SIGNAL(released()), this, SLOT(PressedOnEqualButtonDirectly()));
    connect(ui->Button_MemClear, SIGNAL(released()), this, SLOT(MemoryClear()));
    connect(ui->Button_MemAdd, SIGNAL(released()), this, SLOT(MemoryAdd()));
    connect(ui->Button_MemGet, SIGNAL(released()), this, SLOT(MemoryGet()));
    connect(ui->Button_Pi, SIGNAL(released()), this, SLOT(PiPressed()));
    connect(ui->Button_Euler, SIGNAL(released()), this, SLOT(EulersNumberPressed()));
    connect(ui->Button_Rand, SIGNAL(released()), this, SLOT(RandomNumberPressed()));
    connect(ui->Button_DecimalPoint, SIGNAL(released()), this, SLOT(DecimalPointPressed()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){

    QPushButton *button = static_cast<QPushButton *>(sender());
    QString butVal = button->text();
    QString displayVal = ui->Display->text();
    QRegExp reg("[-]?[0-9.]*");

    if((displayIsEmpty() || operatorTrigger) && (isWhole || operationsBeforePressingEqualCount > 1)){ // Display is empty and decimal point was NOT pressed
        ui->Display->setText(butVal);
    }

    // Adding zeroes onto existing number after decimal place (converting to double after this would yield the same number as before)
    // Example: static_cast<double>(25.560) == 25.56
    // So treat it as a string
    else if(butVal == "0" && !isWhole){
        QString newVal = displayVal + butVal;
        ui->Display->setText(newVal);
    }
    else { // After decimal but not 0
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
    operatorTrigger = false;
}

void Calculator::MathButtonPressed(){
    operationsInARowCount++;
    isWhole = true;
    QPushButton *button = static_cast<QPushButton *>(sender());
    QString butVal = button->text();

    if(!operatorTrigger) operationsBeforePressingEqualCount++; // 2 + 2 + (change to -) 2 (press equal) would be 2 operations before pressing equal

    if(operationsBeforePressingEqualCount > 1) {
        EqualButton();
        GetMathButton();
    } else{
        GetMathButton();
        ui->Display->setText(butVal);
    }
     operatorTrigger = true;
}
bool Calculator::displayIsEmpty(){
    QString displayVal = ui->Display->text();
    return (displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0);
}

void Calculator::EqualButton(){
    operationsInARowCount = 0;
    if (operationsBeforePressingEqualCount < 2) operationsBeforePressingEqualCount = 0;
    double solution = 0.0;
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    if(addTrigger || subTrigger || multTrigger || divTrigger){
        if(addTrigger){
            solution = calcVal + dblDisplayVal;
        } else if(subTrigger){
            solution = calcVal - dblDisplayVal;
        } else if(multTrigger){
            solution = calcVal * dblDisplayVal;
        }  else{
            solution = calcVal / dblDisplayVal;
        }
        ui->Display->setText(QString::number(solution));
    }

    ClearOperatorTriggers();

    isWhole = (solution - static_cast<int>(solution) == 0);

}

void Calculator::ChangeNumberSign(){
    QString displayVal = ui->Display->text();
    QRegExp reg("[-]?[0-9.]*");

    if(reg.exactMatch(displayVal)){
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = dblDisplayVal * -1;
        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}

void Calculator::ClearButtonPressed(){
    ui->Display->setText("0");
    ClearAllTriggers();
    operationsBeforePressingEqualCount = 0;
}

void Calculator::MemoryAdd(){
    QString displayVal = ui->Display->text();
    memory = displayVal.toDouble();
}

void Calculator::MemoryClear(){ 
    memory = 0.0;
}

void Calculator::MemoryGet(){
    ui->Display->setText(QString::number(memory));
    isWhole = (memory - static_cast<int>(memory) == 0);
}

void Calculator::PiPressed(){
    ui->Display->setText(QString::number(PI));
    isWhole = false;
}

void Calculator::EulersNumberPressed(){
    ui->Display->setText(QString::number(EULER));
    isWhole = false;
}

void Calculator::RandomNumberPressed(){
    double max = 1.;
    double min = 0.;

    // Generates random number from [min, max)
    double r = min + (rand() / ( RAND_MAX / (max - min) ) ) ;

    ui->Display->setText(QString::number(r));
}

void Calculator::DecimalPointPressed(){
    QString butVal = ".";
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();
    isWhole = (dblDisplayVal - static_cast<int>(dblDisplayVal) == 0);


    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)) ui->Display->setText("0.");
    else if(isWhole){
        QString newVal = displayVal + butVal;
        ui->Display->setText(newVal);
    }

    isWhole = false;
}

void Calculator::ClearAllTriggers(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    isWhole = true;
    operatorTrigger = false;

}

void Calculator::ClearOperatorTriggers(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
}

void Calculator::GetMathButton(){
    QString displayVal = ui->Display->text();

    QPushButton *button = static_cast<QPushButton *>(sender());
    QString butVal = button->text();

    if(operationsInARowCount > 1) ClearOperatorTriggers();
    else calcVal = displayVal.toDouble();

    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
    } else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    } else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    } else {
        subTrigger = true;
    }
}

void Calculator::PressedOnEqualButtonDirectly(){
    operationsBeforePressingEqualCount = 0;
}
