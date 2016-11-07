#ifndef PLANET_H
#define PLANET_H

#include <QColor>
#include <QString>
#include <algorithm>
#include <deque>

class Planet
{
public:
    Planet(long double x = 0, long double y = 0, long double vx = 0, long double vy = 0, long double r = 0, long double m = 0,
           QString name = QString(), QColor color = QColor(rand()%256,rand()%256,rand()%256));
    void addForce(long double fx,long double fy);
    void addForce(const Planet &other);
    void update(long double dt);
    
    bool creating = 0;
    bool destroyed = 0;
    QColor color;
    long double x,y;//real coords
    long double r,m;//radius and mass
    long double ax,ay;
    long double vx,vy;//real velocity  
    QString name;
    std::deque<std::pair<long double, long double> > track;
    int trackLength = 0;
    int lastTrackUpdate = 0;
};


#endif // PLANET_H
