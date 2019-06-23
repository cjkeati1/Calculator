#include "calculator.h"
#include "ui_calculator.h"
#include "math.h"
#include "random"

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));
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
    if(((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)) && (!decimalTrigger)){
        ui->Display->setText(butVal);
    }  else if(butVal == "0"){
        QString newVal = displayVal + butVal;
        ui->Display->setText(newVal);
    }
    else {
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));

    }
}

void Calculator::MathButtonPressed(){
    decimalTrigger = false;

    if(!addTrigger && !subTrigger && !multTrigger && !divTrigger){ // No trigger was pushed prior to this one and after the last equal sign
         GetMathButton();
         ui->Display->setText("");
    } else{
        QString displayVal = ui->Display->text();
        calcVal = displayVal.toDouble();
        EqualButton();
        GetMathButton();
    }
}

void Calculator::EqualButton(){
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

    decimalTrigger = (solution - static_cast<int>(solution) != 0);

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
    decimalTrigger = (memory - static_cast<int>(memory) != 0);

}

void Calculator::PiPressed(){
    ui->Display->setText(QString::number(PI));
    decimalTrigger = true;
}

void Calculator::EulersNumberPressed(){
    ui->Display->setText(QString::number(EULER));
    decimalTrigger = true;
}

void Calculator::RandomNumberPressed(){
    double r = (static_cast<double>(rand()) / (INT_MAX)) + 1;

    ui->Display->setText(QString::number(r));
}

void Calculator::DecimalPointPressed(){
    QString butVal = ".";
    QString displayVal = ui->Display->text();

    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){
        ui->Display->setText("0.");
        decimalTrigger = true;
    } else if(!decimalTrigger){
        QString newVal = displayVal + butVal;
        ui->Display->setText(newVal);
        decimalTrigger = true;
    }
}

void Calculator::ClearAllTriggers(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    decimalTrigger = false;
}

void Calculator::ClearOperatorTriggers(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
}

void Calculator::GetMathButton(){
    ClearOperatorTriggers();

    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = static_cast<QPushButton *>(sender());
    QString butVal = button->text();

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
