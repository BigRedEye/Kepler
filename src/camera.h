#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
    Camera(long double x = 0, long double y = 0, long double w = 0, long double h = 0);
    long double x0() const;
    long double y0() const;
    long double x1() const;
    long double y1() const;
    long double dx(int pix) const;
    long double dy(int pix) const;
    void setPos(long double px, long double py);
    void zoomIn(long double zoomK);
    
    long double x,y;
    long double w,h;
    long double zoom;
};

#endif // CAMERA_H
