#include "camera.h"

Camera::Camera(long double x, long double y, long double w, long double h)
               : x(x), y(y), w(w), h(h)
{
    zoom = 1;
}

long double Camera::x0() const
{
    return x - w*zoom/2;
}

long double Camera::y0() const
{
    return y - h*zoom/2;
}

long double Camera::x1() const
{
    return x + w*zoom/2;
}

long double Camera::y1() const
{
    return y + h*zoom/2;
}

void Camera::setPos(long double px, long double py)
{
    x = px, y = py;
}    

void Camera::zoomIn(long double zoomK)
{
    if (zoomK <= 0.)
        return;
    zoom *= zoomK;
}

long double Camera::dx(int pix) const
{
    return w*zoom / (long double)pix;
}

long double Camera::dy(int pix) const
{
    return h*zoom / (long double)pix;
}
