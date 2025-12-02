#ifndef HycPoint

#define HycPoint

#include <cmath>

class Point{
    public:
    long double x, y, z;

    Point():x(0), y(0), z(0){}
    Point(long double a, long double b, long double c): x(a), y(b), z(c){}
    
    //calculate the distance bewteen points
    long double getDist(const Point &a, const Point &b){
        return sqrtl((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z));
    }
    long double getDist(const Point &a){
        return getDist(*this, a);
    }
};

#endif