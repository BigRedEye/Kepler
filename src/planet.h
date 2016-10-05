#ifndef PLANET_H
#define PLANET_H

#include <QColor>

class Planet
{
public:
    Planet(long double x, long double y, long double vx, long double vy, long double r, long double m, QColor color):x(x),y(y),vx(vx),vy(vy),r(r),m(m),color(color){
        ax = ay = 0;
    }
    void addForce(long double fx,long double fy);
    void addForce(const Planet &other);
    void update(long double dt);
    
    bool destroyed = 0;
    QColor color;
    long double x,y;//real coords
    long double r,m;//radius and mass
    long double ax,ay;
    long double vx,vy;//real velocity    
};


#endif // PLANET_H
