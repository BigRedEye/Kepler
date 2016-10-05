#include "planet.h"
#include <cmath>
#include <QDebug>

#define G 1

void Planet::addForce(long double fx,long double fy)
{
    ax += fx / m;
    ay += fy / m;
}

void Planet::addForce(const Planet &other)
{
    long double dx = other.x - this->x;
    long double dy = other.y - this->y;
    long double dr = hypot(dx,dy); // == sqrt(dx*dx + dy*dy) 
    long double multiplier = 1;
    //bounding : multiplier < -1
    if (dr <= this->r + other.r)
        multiplier = -2;
    
    //f = G * m1 * m2 / r^2; fx = f*sin(a) = f * x / r = G * m1 * m2 * x / r^3    
    long double fx = G * m * other.m * dx / (dr * dr * dr);
    long double fy = G * m * other.m * dy / (dr * dr * dr);
    
    addForce(fx * multiplier,fy * multiplier);
}

void Planet::update(long double dt)
{
    vx += ax * dt;
    vy += ay * dt;
        
    x += vx * dt;
    y += vy * dt;
    
    ax = ay = 0;
}
