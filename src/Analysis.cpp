#ifndef HycGPSAnalysis

#define HycGPSAnalysis

#include <vector>
#include <cmath>
#include "Point.cpp"
#include "CoordinateConvert.cpp"
using std::vector;

class Analysis{
    public:

    //get an average point
    Point AvgPt(const vector<Point> &pts){
        long double sx = 0, sy = 0, sz = 0;

        for (auto &i : pts){
            sx += i.x;
            sy += i.y;
            sz += i.z;
        }

        return Point(sx/pts.size(), sy/pts.size(), sz/pts.size());
    }

    //calculate an average distance
    long double AvgDist(const vector<Point> &pts, Point &dp){
        long double sr = 0;

        for (auto &i : pts)
            sr += dp.getDist(i);

        return sr/pts.size();
    }
    long double AvgDist(const vector<Point> &pts){
        Point avgpt = AvgPt(pts);
        return AvgDist(pts, avgpt);
    }

    //calculate the horizontal sd
    //B2 in fact
    long double hSD(const vector<Point> &pts, const Point &dp){
        long double sxm = 0;

        for (auto &i : pts)
            sxm += (i.x - dp.x)*(i.x - dp.x);

        return sqrtl(sxm/pts.size());
    }
    long double hSD(const vector<Point> &pts){
        const Point avgpt = AvgPt(pts);
        return hSD(pts, avgpt);
    }

    //calculate the vertical sd
    //B2 in fact
    long double vSD(const vector<Point> &pts, const Point &dp){
        long double sym = 0;

        for (auto &i : pts)
            sym += (i.y - dp.y)*(i.y - dp.y);

        return sqrtl(sym/pts.size());
    }
    long double vSD(const vector<Point> &pts){
        const Point avgpt = AvgPt(pts);
        return vSD(pts, avgpt);
    }

    //calculate the distance sd
    //B2 in fact
    long double rSD(const vector<Point> &pts, Point &dp){
        long double ad = AvgDist(pts, dp);
        long double sr = 0;

        for (auto &i : pts)
            sr += (dp.getDist(i) - ad)*(dp.getDist(i) - ad);

        return sqrtl(sr/pts.size());
    }
    long double rSD(const vector<Point> &pts){
        Point avgpt = AvgPt(pts);
        return rSD(pts, avgpt);
    }

    //calculate the RMS
    long double RMS(const vector<Point> &pts){
        return AvgDist(pts) + rSD(pts);
    }
    long double RMS(const vector<Point> &pts, Point &dp){
        return AvgDist(pts, dp) + rSD(pts, dp);
    }

    //calculate the 2DRMS
    long double RMS2D(const vector<Point> &pts){
        return AvgDist(pts) + 2*rSD(pts);
    }
    long double RMS2D(const vector<Point> &pts, Point &dp){
        return AvgDist(pts, dp) + 2*rSD(pts, dp);
    }

    //calculate the CEP
    long double CEP(const vector<Point> &pts){
        return AvgDist(pts) + 0.68*rSD(pts);
    }
    long double CEP(const vector<Point> &pts, Point &dp){
        return AvgDist(pts, dp) + 0.68*rSD(pts, dp);
    }

    //calculate the deviation in direction phi(DEG)
    double deviationInDirection(const vector<Point> &pts, const Point &dp, double phi){
        double sl = 0;
        CoordinateConvert CC;
        phi = phi * M_PI / 180;

        for (auto& i : pts){
            Point np = CC.BLH2NED(i, CC.BLH2XYZ(dp));
            double dist = np.x*np.x + np.y*np.y,
                   the = atan(np.y/np.x);
            sl += sqrt(dist*cos(phi - the)*cos(phi - the));
        }

        // return sqrt(sl/pts.size());
        return sl/pts.size();
    }
};

#endif