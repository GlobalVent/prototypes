#ifndef __TESTOUT_FILE_H__
#define __TESTOUT_FILE_H__

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include "TimeSeries.h"

#include <fmtstr.h>

class TestOutFile {
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
std::cout << "testout file" << std::endl;                  
        if (fileNameBase.size() > 0) {
            std::ostringstream os;
            os << fileNameBase << "-" << testName << ".ts_" << floatw(3,4) << dt << ".plt";
            std::string fname=os.str();
            _outFile.open (fname, std::ios::out); 
            if (! _outFile.is_open()) {
                errOut << "(ERROR) output file " << fname  << " is not open" << strerror(errno)  << std::endl;
                return(false);
            }
            return(true);
        }
        return(true);
    }
    void writeTimeSeries(TimeSeries &ts) {
        if (_outFile.is_open()) {
            for (auto it = ts.begin(); it != ts.end(); it++) 
                _outFile << it->first <<  ", " << it->second << std::endl;
        }

    }
protected:
private:
    std::ofstream _outFile;
};


#endif

