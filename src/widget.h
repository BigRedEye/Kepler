#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include <vector>
#include "planet.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    QTimer *timer;
    QGraphicsScene *scene;
    
    std::vector<Planet> planets;
    
    long double camX0,camY0,camX1,camY1;
    
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void updatePhys(long double dt);
    
private:
    Ui::Widget *ui;
    
private slots:
    void update();
};

#endif // WIDGET_H
