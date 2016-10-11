#include "widget.h"
#include "ui_widget.h"
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <ctime>

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
    planets.push_back(Planet(200,0,0,7,10,1,QColor(255,0,0)));
    planets.push_back(Planet(400,0,0,5,10,50,QColor(0,0,255)));
    planets.push_back(Planet(422,0,0,6.4,3,0.1,QColor(0,255,0)));
    
    connect(timer,SIGNAL(timeout()),this,SLOT(update())); //call update every timer tick
    timer->start(1000/frameRate);

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

void Widget::update(bool redrawOnly)
{
    scene->clear();
    //fill black
    scene->addRect(0,0,scene->width(),scene->height(),QPen(QColor(0,0,0)),QBrush(QColor(0,0,0)));
    
    if (!redrawOnly)
        updatePhys(timer->interval() * 0.001 * 10); // dt in secs, timer->interval() in msecs    
        
    //screen coords = (real x / dx - camY1, real y / dy - camY0)
    long double dx = (camX1 - camX0) / (1. * scene->width());
    long double dy = (camY1 - camY0) / (1. * scene->height());
    
    for (int i = 0; i<planets.size(); ++i)
    {
        Planet p = planets[i];
        //draw a planet
        QPen pen;
        if (i == chosenPlanet)
            pen = QPen(QBrush(QColor(255,255,255)),4);
        else
            pen = QPen(p.color);
        scene->addEllipse((p.x - camX0) / dx - p.r / dy, ((p.y - camY0) / dy - p.r / dy),
                          2 * p.r / dx, 2 * p.r / dy,pen,QBrush(p.color));
    }  
    updatePlanetInfo();
    updatePlanetsList();
    ui->graphicsView->setScene(scene);
}


void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->x() > ui->graphicsView->width() - 1)
        return;
    long double dx = (camX1 - camX0) / (1. * scene->width());
    long double dy = (camY1 - camY0) / (1. * scene->height());
 
    chosenPlanet = -1;
    for (int i = 0; i<planets.size(); ++i)
        if (hypot(event->x()-(planets[i].x - camX0) / dx,event->y()-(planets[i].y - camY0) / dy) <= planets[i].r / dy)
            chosenPlanet = i;
    update(1);
}

void Widget::updatePlanetsList()
{
        
}
    
void Widget::updatePlanetInfo()
{
    ui->currentPlanetGroupBox->setDisabled(isPlaying);
    ui->currentPlanetGroupBox->setTitle(chosenPlanet == -1 ? "New planet" : "Chosen planet");
    ui->colorBox->setTitle((isPlaying ? "" : "Color"));
    
    ui->pushButton->setDisabled(chosenPlanet == -1);
    ui->pushButton_3->setDisabled(chosenPlanet == -1);
    if (chosenPlanet != -1)
    {
        Planet &p = planets[chosenPlanet];
        ui->px->setValue(p.x);
        ui->py->setValue(p.y);
        ui->pvx->setValue(p.vx);
        ui->pvy->setValue(p.vy);
        ui->pr->setValue(p.r);
        ui->pm->setValue(p.m);
        
        ui->rgbr->setValue(p.color.red());
        ui->rgbg->setValue(p.color.green());
        ui->rgbb->setValue(p.color.blue());
        
        ui->lineEdit->setText(p.name);
    }
}

void Widget::setPlanetInfo()
{
    Planet &p = planets[chosenPlanet];
    
    p.x = ui->px->value();
    p.y = ui->py->value();
    p.vx = ui->pvx->value();
    p.vy = ui->pvy->value();
    
    p.r = ui->pr->value();
    p.m = ui->pm->value();
    
    p.color = QColor(ui->rgbr->value(),
                     ui->rgbg->value(),
                     ui->rgbb->value());
    p.name = ui->lineEdit->text();
    
    update(1);
}


void Widget::on_playButton_clicked()
{
    isPlaying = !isPlaying;
    
    if (isPlaying)
    {
        ui->playButton->setText("Stop");
        timer->start(1000/frameRate);
    }
    else
    {
        updatePlanetInfo();
        ui->playButton->setText("Start");
        timer->stop();
    }
}

void Widget::on_pushButton_clicked()
{
    setPlanetInfo();
}

void Widget::on_pushButton_2_clicked()
{
    chosenPlanet = planets.size();
    planets.push_back(Planet(0,0,0,0,0,0));
    setPlanetInfo();
    update(1);
}

void Widget::on_pushButton_3_clicked()
{
    planets.erase(planets.begin() + chosenPlanet);
    update(1);
}
