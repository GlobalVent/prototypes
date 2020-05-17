#ifndef __PARSE_TEST_ARGS_H__
#define __PARSE_TEST_ARGS_H__

//#include <iostream>
#include <string>
#include <vector>
#include <ostream>

class ParseTestArgs {
public:
    std::vector<std::string> parms;
    std::string outFileName;
    bool help;
    unsigned int verbose;
    ParseTestArgs(std::ostream &out = std::cerr) :
      _out(out),
      help(false),
      verbose(false)
       {};
    /**
     * @brief      parse -- parse arguments.
     *
     * @param[in]  argc  The count of arguments
     * @param      argv  The arguments array
     * bool -- true if parse was successful,
     *         false if not...
     *         error out put driven outstream
     */
    bool parse(int argc, const char * argv []) {
        for (auto c = 1; c < argc; c++ ) {
            auto cp = argv[c];
            if (cp[0] == '-') {
                cp++;
                if (*cp == '-')
                    cp++;
                std::string sopt = std::string(cp);
                std::vector<std::string> t;
                tokenize(t,cp,'=');
                if (sopt == "help") {
                    return(true);
                }
                if (t[0] == "verbose") {
                    verbose++;
                    continue;
                }
                else if (t[0] == "outfile") {
                    if (t.size() != 2) {
                        _out << "missing option for --outfile=" << std::endl;
                        return(false);
                    }
                    outFileName=t[1];
                }
                else {
                    _out << "unrecognized option: " << argv[c] << std::endl;
                    return(false);
                }
            }
            else {
                parms.push_back(cp);
            }
        }
        return(true);
    };
protected:
    std::ostream &_out;
private:
    void tokenize(std::vector<std::string> &t, 
                  std::string const &str, 
                  const char delim) {
        size_t start;
        size_t end = 0;
        while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
        {
            end = str.find(delim, start);
            t.push_back(str.substr(start, end - start));
        }
    };

};


#endif

