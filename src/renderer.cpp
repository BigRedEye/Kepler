#include "renderer.h"
#include <QDebug>

Renderer::Renderer(QWidget *parent)
                  : QGraphicsView(parent)
{
    w = this->width() - 2;
    h = this->height() - 2;
    scene = new QGraphicsScene(0,0,w,
                               h,this);
}

void Renderer::newGeometry(int _x, int _y, int _w, int _h)
{
    this->setGeometry(_x,_y,_w,_h);
    
    delete scene;
    w = _w - 2, h = _h - 2;
    scene = new QGraphicsScene(0,0,w,
                               h,this);
}

void Renderer::redraw(const std::vector<Planet> &planets, const Camera &cam, int chosenPlanet, const Planet &newPlanet)
{
    scene->clear();
    
    //fill black
    scene->addRect(0,0,scene->width(),scene->height(),QPen(QColor(0,0,0)),QBrush(QColor(0,0,0)));
        
    long double dx = cam.dx(scene->width());
    long double dy = cam.dy(scene->height());
    
    for (size_t i = 0; i<planets.size(); ++i)
    {
        Planet p = planets[i];
        QPen pen;
        
        //draw a track
        if (i == chosenPlanet)
        {
            int len = std::min(p.trackLength,(int)p.track.size());
            QColor col = p.color;
            long double r = col.red(),
                g = col.green(),
                b = col.blue();
            long double dr = r / (len+1.),
                        dg = g / (len+1.),
                        db = b / (len+1.);
            for (int j = 0; j<len-1; ++j)
            {
                scene->addLine(((p.track[j].first - cam.x0())) / dx,
                                (p.track[j].second - cam.y0()) / dy,
                                (p.track[j+1].first - cam.x0()) / dx,
                                (p.track[j+1].second - cam.y0()) / dy,
                                QPen(QBrush(QColor((int)r,(int)g,(int)b)),1));
                r -= dr, g -= dg, b -= db;
            }
        }
        //draw a planet
        if ((int)i == chosenPlanet)
            pen = QPen(QBrush(QColor(255,255,255)),4);
        else
            pen = QPen(p.color);
        scene->addEllipse((p.x - cam.x0()) / dx - p.r / dx,
                          ((p.y - cam.y0()) / dy - p.r / dy),
                          2 * p.r / dx, 2 * p.r / dy,pen,QBrush(p.color));
    }
    Planet p = newPlanet;
    scene->addEllipse((p.x - cam.x0()) / dx - p.r / dx,
                      ((p.y - cam.y0()) / dy - p.r / dy),
                      2 * p.r / dx, 2 * p.r / dy,QPen(p.color),QBrush(p.color));
    this->setScene(scene);
}

void Renderer::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMove(event);
}


void Renderer::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseRelease(event);
}
