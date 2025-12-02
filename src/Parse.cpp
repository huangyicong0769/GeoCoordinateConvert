#ifndef HycParse

#define HycParse

#include "Point.cpp"
#include <iostream>
#include <string>
#include <cstdio>
using std::string;

class GGAParse{
    public:

    long double DMS2D(const double &d, const double &m, const double &s){
        return d + m/60 + s/3600;
    }

    //judge if an input is in GGA format
    bool isGGA(const string &str){
        const char *cstr = "GGA";

        if (str[0] != '$') return 0;
        for (int i = 0, j = 0; i < str.size(); i++){
            if (str[i] == cstr[j]){
                if (++j >= 3) return 1;
            }else j = 0;
            if (str[i] == ',') break;
        }
        return 0;
    }

    //extrat LLA point from GGA info
    //if return (0,0), meaning it is invaild
    Point getPt(const string &str){
        int cnt = 0, td1 = 0, td2 = 0, td3 = 0, td4 = 0, td5 = 0, sg1 = 1, sg2 = 1, i = 0;

        for (auto &ch : str){
            if (ch == ',') {
                cnt++;
                i = 0;
                continue;
            }
            if (cnt == 2){
                if (ch == '.') continue;
                if (i < 2) 
                    td1 = td1*10 + ch - '0';
                else td2 = td2*10 + ch - '0';
            }
            if (cnt == 3){
                if (ch == 'N') sg1 = 1;
                if (ch == 'S') sg1 = -1;
            }
            if (cnt == 4){
                if (ch == '.') continue;
                if (i < 3) 
                    td3 = td3*10 + ch - '0';
                else td4 = td4*10 + ch - '0';
            }
            if (cnt == 5){
                if (ch == 'E') sg2 = 1;
                if (ch == 'W') sg2 = -1;
            }
            if (cnt == 9){
                //doesn't needed so far
            }
            if (cnt > 9) break;
            i++;
        }

        return Point{sg1*DMS2D(td1, (double)td2/10000.0, 0),
                     sg2*DMS2D(td3, (double)td4/10000.0, 0),
                     (double)td5};
    }

    //extrat UTC time from GGA info as HHmmss.sss => HHmmsssss
    int getTime(const string &str){
        int cnt = 0, time = 0;

        for (auto &ch : str){
            if (ch == ',') {
                cnt++;
                continue;
            }
            if (cnt == 1){
                if (ch == '.') continue;
                time = time*10 + ch - '0';
            }
            if (cnt > 1) break;
        }

        return time;
    }

    //extrat HDOP from GGA info as X.X => XX
    double getHDOP(const string &str){
        int cnt = 0, cnt2 = 0, dot = 0;
        double hdop = 0;

        for (auto &ch : str){
            if (ch == ',') {
                cnt++;
                continue;
            }
            if (cnt == 8){
                if (ch == '.') {
                    cnt2 = 0;
                    dot = 1;
                    continue;
                }
                hdop = hdop*10 + ch - '0';
                cnt2 += dot;
            }
            if (cnt > 8) break;
        }

        return hdop / pow(10, cnt2);
    }
};

// testcode
// int main(){
//     string s;
//     std::cin >> s;

//     GGAParse prs;

//     std::cout << prs.isGGA(s) << std::endl << prs.getHDOP(s) << std::endl;
// }

#endif