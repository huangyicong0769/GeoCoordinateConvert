#ifndef HycCrdCvr

#define HycCrdCvr

#include <cmath>
#include <vector>
using std::vector;
using std::move;

#include "Point.cpp"

class CoordinateConvert{
    const long double a = 6378137.0, f = 1/298.257222101,
                      b = a -  a * f,
                      e2 = f*(2.0 - f),
                      DEG2REG = M_PI / 180;
    public:
    //convert LLA into ECEF
    Point BLH2XYZ(const Point &p){
        //lamda is lagtitude, phi is longtitude
        const long double lamda = p.x * DEG2REG, phi = p.y * DEG2REG, &h = p.z;
        long double N = a/sqrtl(1 - e2*sinl(lamda)*sinl(lamda));
        return Point{(N+h)*cosl(lamda)*cosl(phi),
                     (N+h)*cosl(lamda)*sinl(phi),
                     (N*(1-e2)+h)*sinl(lamda)};
    }

    //convert LLA into ENU, dp in ECEF
    Point BLH2NED(const Point &p, const Point &dp){
        //lamda is lagtitude, phi is longtitude, dp is datum point
        const long double lamda = p.x * DEG2REG, phi = p.y * DEG2REG, &h = p.z;
        const Point tp = BLH2XYZ(p); // as temporary point
        const long double &x = tp.x, &y = tp.y, &z = tp.z, &x0 = dp.x, &y0 = dp.y, &z0 = dp.z;
        return Point{(x - x0)*(-sinl(lamda)*cosl(phi)) + (y - y0)*(-sinl(lamda)*sinl(phi)) + (z - z0)*(cosl(lamda)),
                     (x - x0)*(-sinl(phi)) + (y - y0)*(cosl(phi)),
                     (x - x0)*(-cosl(lamda)*cosl(phi)) + (y - y0)*(-cosl(lamda)*sinl(phi)) + (z - z0)*(-sinl(lamda))};
    }

    //convert LLA into ENU in a more inaccurate but easier way
    Point BLH2NEDK(const Point &p, const Point &dp){
        const long double lamda0 = dp.x * DEG2REG, phi0 = dp.y * DEG2REG;
        const long double lamda = p.x * DEG2REG, phi = p.y * DEG2REG;
        const long double Kx = a*cosl(lamda0)/powl(1-e2*powl(sinl(lamda0), 2), 0.5),
                          Ky = a*(1-e2)/powl(1-e2*powl(sinl(lamda0), 2), 1.5);
        return Point{Kx*(phi - phi0), Ky*(lamda - lamda0), 0};
    }

    vector<Point> BLH2XYZ(const vector<Point> &pts){
        vector<Point> npts;
        
        for (auto& pt : pts)
            npts.push_back(BLH2XYZ(pt));

        return move(npts);
    }

    vector<Point> BLH2NED(const vector<Point> &pts, const Point& dpt){
        vector<Point> npts;
        
        for (auto& pt : pts)
            npts.push_back(BLH2NED(pt, dpt));

        return move(npts);
    }

    // vector<Point> BLH2NED(const vector<Point> &pts){
    //     vector<Point> npts;
    //     Analysis Ays;
    //     return BLH2NED(pts, Ays.AvgPt(pts));
    // }
};

//testcode
// #include <iostream>
// #include <iomanip>
// #include <cstdlib>
// using namespace std;

// int main(){
//     long double a, b;
//     cout << "Input the datum point in LLA as DEG:"; cin >> a >> b;
//     Point dp{a, b, 0};
//     cout << "Input the point in LLA as DEG:"; cin >> a >> b;
//     Point p{a, b, 0};

//     CoordinateConvert CC;
//     Point np = CC.BLH2NED(p, CC.BLH2XYZ(dp));
//     cout << "A:" << setprecision(8) << np.x << ' ' << np.y << ' ' << np.z << endl;
//     np = CC.BLH2NEDK(p, dp);
//     cout << "B:" << setprecision(8) << np.x << ' ' << np.y << ' ' << np.z << endl;
//     system("pause");
// }

#endif