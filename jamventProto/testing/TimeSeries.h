#ifndef __TIME_SERIES_H__
#define __TIME_SERIES_H__

#include <vector>
#include <algorithm>


class TimeSeries
{
protected:
    typedef std::pair<double, double>     TimeSeriesPair_t;
    typedef std::vector<TimeSeriesPair_t> TimeSeriesVector_t;
    TimeSeriesVector_t ts;

public:
    typedef TimeSeriesVector_t::iterator iterator;
    typedef TimeSeriesVector_t::const_iterator const_iterator;
    iterator begin() { return ts.begin();}
    iterator end()   { return ts.end();  }
    TimeSeriesPair_t &back()  { return ts.back(); }
    void push_back(double a,double b) { ts.push_back(TimeSeriesPair_t(a,b)); }
    iterator findTime(double time, double precision) {
        auto it = std::find_if( ts.begin(), ts.end(),
          [time, precision](const TimeSeriesPair_t& e){ 
            return (fabs(e.first - time) < precision);
            });
        return(it);
    }
private:
};

#endif
