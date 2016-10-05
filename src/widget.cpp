#include "widget.h"
#include "ui_widget.h"
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer();
    scene = new QGraphicsScene(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2,ui->graphicsView);
    
    camX0 = -800, camY0 = -600, camX1 = 800, camY1 = 600;

    //Planet (x,y,vx,vy,radius,mass,QColor(r,g,b)
    planets.push_back(Planet(0,0,0,0,100,10000,QColor(255,255,0)));
    planets.push_back(Planet(200,0,0,7,10,0.01,QColor(255,0,0)));
    planets.push_back(Planet(400,0,0,5,10,50,QColor(0,0,255)));
    planets.push_back(Planet(422,0,0,6.4,3,0.01,QColor(0,255,0)));
    
    connect(timer,SIGNAL(timeout()),this,SLOT(update())); //call update every timer tick
    timer->start(1000/60); //60 fps

}

Widget::~Widget()
{
    delete ui;
}

void Widget::updatePhys(long double dt)
{
    for (int i = 0; i<planets.size(); ++i)
    {
        for (int j = 0; j<planets.size(); ++j)
        {
            if (i == j)
                continue;
            planets[i].addForce(planets[j]);
        }
        planets[i].update(dt);
   }
}

void Widget::update()
{
    scene->clear();
    //fill black
    scene->addRect(0,0,scene->width(),scene->height(),QPen(QColor(0,0,0)),QBrush(QColor(0,0,0)));
    
    updatePhys(timer->interval() * 0.001 * 10); // dt in secs, timer->interval() in msecs    
    
    //screen coords = (real x / dx - camY1, real y / dy - camY0)
    long double dx = (camX1 - camX0) / (1. * scene->width());
    long double dy = (camY1 - camY0) / (1. * scene->height());
    
    for (int i = 0; i<planets.size(); ++i)
    {
        Planet p = planets[i];
        //draw a planet
        scene->addEllipse((p.x - camX0) / dx - p.r / dy, ((p.y - camY0) / dy - p.r / dy),
                          2 * p.r / dx, 2 * p.r / dy,QPen(p.color),QBrush(p.color));
    }  
    ui->graphicsView->setScene(scene);
}
