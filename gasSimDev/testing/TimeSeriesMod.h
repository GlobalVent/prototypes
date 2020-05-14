#ifndef __TIME_SERIES_MOD_H__
#define __TIME_SERIES_MOD_H__

#include <vector>
#include <algorithm>    // std::find_if
#include <math.h>


class TimeSeriesMod
{
protected:
    struct Tpv{
        Tpv() = delete;
        Tpv(double _t, double _pRes, double _pSys, double _pO2, double _lvol,
            bool _valveA, bool _valveB, bool _valveC, bool _valveD) :
          t(_t), pRes(_pRes), pSys(_pSys), pO2(_pO2), lvol(_lvol),
          valveA(_valveA), valveB(_valveB), valveC(_valveC), valveD(_valveD) {};
        double  t;
        double  pRes;
        double  pSys;
        double  pO2;
        double  lvol;  
        bool    valveA;
        bool    valveB;
        bool    valveC;
        bool    valveD;
    };
    typedef std::vector<Tpv> TpvVector_t;
    TpvVector_t ts;

public:
    typedef TpvVector_t::iterator iterator;
    typedef TpvVector_t::const_iterator const_iterator;
    iterator begin() { return ts.begin();}
    iterator end()   { return ts.end();  }
    Tpv &back()  { return ts.back(); }
    void push_back(const Tpv &tpv);
    void push_back(double t, double pRes, double pSys, double pO2, double lvol,
                   bool valveA, bool valveB, bool valveC, bool valveD) { 
                       ts.push_back(Tpv(t, pRes, pSys, pO2, lvol, valveA, valveB, valveC, valveD )); 
    };
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
