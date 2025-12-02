#ifndef HycGPSData

#define HycGPSData

#include <vector>
#include <cstdio>
using std::vector;

#include "Point.cpp"
#include "Parse.cpp"

class Data{
    vector<Point> pts;
    double avgHDOP;

    public:
    Data(){
        avgHDOP = 0;
        pts.clear();
        pts.shrink_to_fit();
    }

    //expose
    vector<Point>& data(){
        return pts;
    } 

    //as name
    void inputFromGGA(const char* name){
        freopen(name, "r", stdin);

        avgHDOP = 0;
        char str[100];
        GGAParse Prs;
        int orgTistp = -1;
        while (scanf("%s", str) != EOF){
            if (!Prs.isGGA(str)) continue;

            //throw out datas in the first five mins
            if (orgTistp == -1)
                orgTistp = Prs.getTime(str);
            if (Prs.getTime(str) - orgTistp < 500000) continue;

            //throw out low-quality datas
            if (Prs.getHDOP(str) > 1.1) continue;

            avgHDOP += Prs.getHDOP(str);

            pts.push_back(Prs.getPt(str));
        }

        avgHDOP /= pts.size();

        fclose(stdin);
    }

    double quality(){
        return avgHDOP;
    }
};

#endif