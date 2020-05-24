#ifndef __TIME_SERIES_PVO2_H__
#define __TIME_SERIES_PVO2_H__

#include <vector>
#include <algorithm>    // std::find_if
#include <math.h>


class TimeSeriesPVO2
{
protected:
    struct Tpv{
        Tpv() = delete;
        Tpv(double _t, double _p, double _v,double _o2) :
          t(_t), p(_p), v(_v), o2(_o2) {};
        double t;
        double p;
        double v;
        double o2;
    };
    typedef std::vector<Tpv> TpvVector_t;
    TpvVector_t ts;

public:
    typedef TpvVector_t::iterator iterator;
    typedef TpvVector_t::const_iterator const_iterator;
    iterator begin() { return ts.begin();}
    iterator end()   { return ts.end();  }
    Tpv &back()  { return ts.back(); }
    void push_back(double t, double p, double v, double o2) { ts.push_back(Tpv(t,p,v,o2)); }
    iterator findTime(double time, double precision) {
        auto it = std::find_if( ts.begin(), ts.end(),
          [time, precision](const Tpv& e){ 
            return (fabs(e.t - time) < precision);
            });
        return(it);
    };
private:
};

#endif
