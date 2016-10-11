#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QLineEdit>
#include <QVector>

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
    
    void updatePlanetInfo();
    
    void updatePlanetsList();
    
    void changePlanetInfo();
    
    void setPlanetInfo();
    
private:
    bool editing = 0;
    
    Ui::Widget *ui;
    
    bool isPlaying = 1;
    
    int chosenPlanet = -1;
    
    int frameRate = 100;
    
    QVector<QLineEdit> planetList;    
private slots:
    void update(bool redrawOnly = 0);
    
    void on_playButton_clicked();
    
    void mousePressEvent(QMouseEvent *event);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // WIDGET_H
