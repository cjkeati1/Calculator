#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include <QWidget>
namespace Ui {
class test;
}
class test : public QMainWindow
{
    Q_OBJECT
public:
    explicit test(QWidget *parent = nullptr);
Ui::test *ui;
signals:

public slots:
};

#endif // TEST_H
