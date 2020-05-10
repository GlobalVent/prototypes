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
            os << fileNameBase << "-" << testName << ".ts_" << floatw(3,4) << dt << ".plt";
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
                << "set yrange [0:]" << std::endl
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
                << "set yrange [0:]" << std::endl
                << "plot $data using 1:2 title 'pressure' with lines, $data using 1:3 title 'volume' with lines " 
                << std::endl;
        }
    }
protected:
private:
    std::ofstream _outFile;
};


#endif

