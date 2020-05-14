#ifndef __TimeSeriesOutFile_H__
#define __TimeSeriesOutFile_H__

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>

#include <iostream>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>
 

#include "TimeSeries.h"
#include "TimeSeriesPV.h"
#include "TimeSeriesPVO2.h"
#include "TimeSeriesMod.h"

#include <fmtstr.h>

class TimeSeriesOutFile {
public:
    /**
     * @brief open the output file
     * 
     * @param fileNameBase -- may be NULL in which case don't open the file.
     * @param testName -- name of data file to write.
     * @param dt -- delta t used for this data set.
     * @param errOut -- stream to send any errors to...
     * @return true -- open successfully
     * @return false -- open failed
     */
    bool open(std::string const &fileNameBase, 
              std::string const &testName,
              double dt,
              std::ostream &errOut) {

        if (fileNameBase.size() > 0) {
            std::ostringstream os;
            os << fileNameBase << "_" << testName << ".ts_" << floatw(3,4) << dt << ".plt";
            std::string fname=os.str();
            _outFile.open (fname, std::ios::out); 
            if (! _outFile.is_open()) {
                errOut << "(ERROR) output file " << fname  << " is not open" << std::strerror(errno)  << std::endl;
                return(false);
            }
            return(true);
        }
        return(true);
    }
    void writeTimeSeries(TimeSeries &ts) {
        if (_outFile.is_open()) {
            _outFile << "$data <<EOD" << std::endl;
            for (auto it = ts.begin(); it != ts.end(); it++) 
                _outFile << it->first <<  ", " << it->second << std::endl;
            _outFile << "EOD" << std::endl
                << "set xlabel 'time'" << std::endl
                << "set ylabel 'bars'" << std::endl
                << "set autoscale y" << std::endl
                //<< "set yrange [0:]" << std::endl
                << "plot $data using 1:2 title 'pressure' with lines" 
                << std::endl;
                
        }
    }
    void writeTimeSeries(TimeSeriesPV &ts) {
        if (_outFile.is_open()) {
            _outFile << "$data << EOD" << std::endl;
            for (auto it = ts.begin(); it != ts.end(); it++) 
                _outFile << it->t <<  ", " 
                         << it->p << ", " 
                         << it->v << std::endl;
            _outFile << "EOD" << std::endl
                << "set xlabel 'time' " << std::endl
                << "set ylabel 'bars/liters' " << std::endl
                //<< "set yrange [0:]" << std::endl
                << "set autoscale y" << std::endl
                << "plot $data using 1:2 title 'pressure' with lines, $data using 1:3 title 'volume' with lines " 
                << std::endl;
        }
    }
    void writeTimeSeries(TimeSeriesPVO2 &ts) {
        if (_outFile.is_open()) {
            _outFile << "$data << EOD" << std::endl;
            for (auto it = ts.begin(); it != ts.end(); it++) 
                _outFile << it->t <<  ", " 
                         << it->p << ", " 
                         << it->v << ", "
                         << it->o2 << ", "
                         << std::endl;
            _outFile << "EOD" << std::endl
                << "set xlabel 'time' " << std::endl
                << "set ylabel 'bars/liters' " << std::endl
                //<< "set yrange [1:]" << std::endl
                << "set autoscale y" << std::endl
                << "plot $data using 1:2 title 'pressure' with lines, "
                << "$data using 1:3 title 'volume' with lines, " 
                << "$data using 1:4 title 'pO2' with lines "
                << std::endl;
        }
    }
    void writeTimeSeries(TimeSeriesMod &ts) {
        if (_outFile.is_open()) {
            _outFile << "$data << EOD" << std::endl;
            for (auto it = ts.begin(); it != ts.end(); it++) 
                _outFile << floatw(4) << it->t <<  ", " 
                         << floatw(4) << it->pRes << ", " 
                         << floatw(4) << it->pSys << ", "
                         << floatw(4) << it->pO2 << ", "
                         << floatw(4) << it->lvol << ", "
                         << decw(0) << it->valveA << ", "
                         << it->valveB << ", "
                         << it->valveC << ", "
                         << it->valveD 
                         << std::endl;
            _outFile << "EOD" << std::endl
                << "set multiplot layout 4,1 rowsfirst" << std::endl
                << "set xlabel 'time' " << std::endl
                << "set ylabel 'bars/liters' " << std::endl
                //<< "set yrange [0:]" << std::endl
                << "set autoscale y" << std::endl
                << "plot $data using 1:2 title 'pRes' with lines "  << std::endl
                << "plot $data using 1:3 title 'pSys' with lines, " << std::endl
                << "plot $data using 1:4 title 'pO2' with lines, "  << std::endl
                << "plot $data using 1:5 title 'lvol' with lines, " << std::endl
                << std::endl;
        }
    }
protected:
private:
    std::ofstream _outFile;
};


#endif

