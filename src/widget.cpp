#include "widget.h"
#include "ui_widget.h"
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QColorDialog>
#include <ctime>
#include <utility>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    renderer = new Renderer(this);
    renderer->newGeometry(0,0,800,600);
    timer = new QTimer();
    editor = new PlanetEditor();
    
    cam = Camera(0,0,800*2,600*2);

    //Planet (x,y,vx,vy,radius,mass,QColor(r,g,b)
    addPlanet(Planet(0,0,0,0,100,10000,"planet 1",QColor(255,255,0)));
    addPlanet(Planet(200,0,0,7,10,1,"planet 2",QColor(255,0,0)));
    addPlanet(Planet(400,0,0,5,10,50,"planet 3",QColor(0,0,255)));
    addPlanet(Planet(422,0,0,6.4,3,0.1,"planet 4",QColor(0,255,0)));
    
    connect(editor,SIGNAL(creation_finished(bool)),this,SLOT(creation_finished(bool)));
    connect(renderer,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(renderer,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(mouseReleaseEvent(QMouseEvent*)));
    connect(timer,SIGNAL(timeout()),this,SLOT(update())); //call update every timer tick
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(change_current_planet(QListWidgetItem*)));
    timer->start(1000/frameRate);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updatePhys(long double dt)
{
    for (size_t i = 0; i<planets.size(); ++i)
    {
        for (size_t j = 0; j<planets.size(); ++j)
        {
            if (i == j)
                continue;
            planets[i].addForce(planets[j]);
            
            ++planets[i].lastTrackUpdate;
            
            
            if (planets[i].lastTrackUpdate > 50)
            {
                planets[i].track.push_front(std::make_pair(planets[i].x, planets[i].y));
                if (planets[i].track.size() > planets[i].trackLength)
                    planets[i].track.pop_back();
                planets[i].lastTrackUpdate = 0;
            }
        }
        planets[i].update(dt);
   }
}

void Widget::update(bool redrawOnly)
{
    if (chosenPlanet != -1 && ui->checkBox->isChecked())
        cam.setPos(planets[chosenPlanet].x,planets[chosenPlanet].y);
   
    if (!redrawOnly)
        updatePhys(timer->interval() * 0.001 * 10); // dt in secs, timer->interval() in msecs    
    
    renderer->redraw(planets,cam,chosenPlanet,newPlanet);
    
    updatePlanetInfo();
    updateCamInfo();
}


void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->x() > ui->graphicsView->width() - 1)
        return;
    
    long double dx = cam.dx(renderer->w);
    long double dy = cam.dy(renderer->h);
    if (event->button() & Qt::LeftButton)
    {
     
        chosenPlanet = -1;
        for (size_t i = 0; i<planets.size(); ++i)
            if (hypot(event->x()-(planets[i].x - cam.x0()) / dx,
                      event->y()-(planets[i].y - cam.y0()) / dy) <= planets[i].r / dy)
                chosenPlanet = i;
    }
    else if (event->buttons() & Qt::MidButton)
    {
        mousePos = event->pos();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        newPlanet = Planet();
        newPlanet.creating = 1;
        newPlanet.x = cam.x0() + (event->x() * dx);
        newPlanet.y = cam.y0() + (event->y() * dy);
        newPlanet.m = 1;
        newPlanet.name = QString("New planet");
    }
    update(1);
}
   \

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    long double dx = cam.dx(renderer->w);   
    long double dy = cam.dy(renderer->h);
    
    if (event->buttons() & Qt::MiddleButton)
    {
        QPoint pos = event->pos();
        pos.setX(std::max(std::min(pos.x(),(int)ui->graphicsView->width()-1),1));
        pos.setY(std::max(std::min(pos.y(),(int)ui->graphicsView->height()-1),1));
        
        
        cam.x -= (pos.x() - mousePos.x()) * dx;
        cam.y -= (pos.y() - mousePos.y()) * dy;
        
        mousePos = pos;
    }
    else if (event->buttons() & Qt::RightButton)
    {
        newPlanet.r = hypot(event->x()-(newPlanet.x - cam.x0()) / dx,
                            event->y()-(newPlanet.y - cam.y0()) / dy) * cam.zoom;
    }
    update(1);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::RightButton)
    {
        if (newPlanet.r > 0)
        {
            newPlanet.m = newPlanet.r * newPlanet.r * newPlanet.r / 100;
            editor->p = newPlanet;
            editor->updatePlanetInfo();
            editor->show();
        }
        else
            newPlanet = Planet();
    }
}

void Widget::addPlanet(const Planet &p)
{
    planets.push_back(p);
    ui->listWidget->addItem(p.name);
}

void Widget::updatePlanetInfo()
{
    ui->currentPlanetGroupBox->setDisabled(isPlaying);
    ui->currentPlanetGroupBox->setTitle(/*chosenPlanet == -1 ? "New planet" : */"Chosen planet");
    ui->colorBox->setTitle((/*isPlaying ? "" : */"Color"));
    
    ui->pushButton->setDisabled(chosenPlanet == -1);
    ui->pushButton_3->setDisabled(chosenPlanet == -1);
    ui->listWidget->setCurrentRow(chosenPlanet);
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
        
        ui->trackLen->setValue(p.trackLength);
        ui->listWidget->currentItem()->setText(p.name);
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
    p.trackLength = ui->trackLen->value();
    
    ui->listWidget->item(chosenPlanet)->setText(p.name);
    
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

void Widget::on_pushButton_3_clicked()
{
    ui->listWidget->takeItem(chosenPlanet);
    planets.erase(planets.begin() + chosenPlanet);
    --chosenPlanet;
    update(1);
}

void Widget::change_current_planet(QListWidgetItem *item)
{
    int i = ui->listWidget->row(item);
    qDebug() << i;
    chosenPlanet = i;
    update(1);
}

void Widget::updateCamInfo()
{
    ui->camH->setText("H = " + QString::number((double)(cam.h * cam.zoom)));
    ui->camW->setText("W = " + QString::number((double)(cam.w * cam.zoom)));
        
    ui->camX->setText("X = " + QString::number((double)cam.x));
    ui->camY->setText("Y = " + QString::number((double)cam.y));
}

void Widget::creation_finished(bool success)
{
    editor->hide();
    if (success)
    {
        newPlanet = editor->p;
        newPlanet.creating = 0;
        addPlanet(newPlanet);
    }
    newPlanet = Planet();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    long double multiplier = 1.0905077326652577; //2 ** 0.125
    if (event->angleDelta().isNull())
        return;
    if (event->angleDelta().y() == -120)
        cam.zoomIn(multiplier);
    if (event->angleDelta().y() == 120)
        cam.zoomIn(1/multiplier);
}
