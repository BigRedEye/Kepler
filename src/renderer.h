#ifndef RENDERER_H
#define RENDERER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QMouseEvent>

#include <vector>

#include "planet.h"
#include "camera.h"

class Renderer : public QGraphicsView
{
Q_OBJECT
    
public:
    Renderer(QWidget *parent = nullptr);
    void newGeometry(int x, int y, int w, int h);
    void redraw(const std::vector<Planet> &v, const Camera &cam, int chosenPlanet, const Planet &newPlanet = Planet());
    
    QGraphicsScene *scene;
    int w,h;  
    
signals:
    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
   
private slots:
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // RENDERER_H
