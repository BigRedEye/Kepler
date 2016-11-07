#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QLineEdit>
#include <QVector>
#include <QListWidgetItem>
#include <QEvent>
#include <queue>
#include <QWheelEvent>

#include <vector>

#include "planet.h"
#include "camera.h"
#include "renderer.h"
#include "planeteditor.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Camera cam;
    QTimer *timer;
    Renderer *renderer; 
    PlanetEditor *editor;
    
    std::vector<Planet> planets;
    Planet newPlanet;
    long double camX0,camY0,camX1,camY1;
    
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void updatePhys(long double dt);
    void updatePlanetInfo();
    void updateCamInfo();
    void changePlanetInfo();
    void setPlanetInfo();
    void addPlanet(const Planet &p);
    
private:
    bool editing = 0, isPlaying = 1;
    Ui::Widget *ui;
    int chosenPlanet = -1;
    int frameRate = 100;
    QPoint mousePos;
    
private slots:
    void update(bool redrawOnly = 0);
    void on_playButton_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void change_current_planet(QListWidgetItem* item);
    void creation_finished(bool);
    void wheelEvent(QWheelEvent *event);
    
};

#endif // WIDGET_H
