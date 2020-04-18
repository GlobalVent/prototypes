#include "GassimNode.h"
#include "vgdbLib01.h"

/*
	To test the library, include "vgdbLib01.h" from an application project
	and call vgdbLib01Test().
	
	Do not forget to add the library to Project Dependencies in Visual Studio.
*/


//
//  main.cpp
//  xcode-test01
//
//  Created by ralph bellofatto on 4/12/20.
//  Copyright © 2020 ralph bellofatto. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <strstream>
#include <istream>
#include <cmath>


#include "StrTokenize.h"
#include "GassimConst.h"
#include "GassimConv.h"
#include "fmtstr.h"

using namespace std;

typedef vector<string> ErrStrs;

/**
 * @brief testGassimConv
 * @details testGassimConvsion
 * @return true if test success, false if failure...
 */
class testGassimConv
{
public:
	bool main();

protected:
	void testConv(const string &file,
		unsigned line, 
		const string &testStr, 
		double v, 
		double exp, 
		ErrStrs &errs);
	void testGasConstantBars(ErrStrs &errs);
	void testGasConstantCmH20(ErrStrs &errs);
private:

};

void testGassimConv::testConv(const string &file, unsigned line, 
			  const string &testStr, 
			  double v, 
			  double exp, 
			  ErrStrs &errs) 
{
    /*
    do converstion test.
    :param loc: location in file of the test.
    :param testStr: test string to display
    :param v: value to check
    :param exp:  expected value
    :param errs: error array to add errors to
    :return:
    */
    if (v != exp) {
		StrTokenize ts = StrTokenize(file,'/');

    	ostrstream os;
    	os << "(ERROR) " << ts.tokens.back() << ":" << line << ": " 
    	   << testStr << "==" << v << " exp=" << exp;
    	errs.push_back(os.str());
    }
}   


void testGassimConv::testGasConstantBars(ErrStrs &errs)
{
    // # the volume of an ideal gas at STP is 22.71 L
    // # PV=nRT
    // # test the value of R against this.
    double P=1;      // # bars
    double V=1;      // # liters
    double R=GassimConst::uGasConst;
    double n=1;
    double T=GassimConv::cToKelvin(0);   // # 0 degress C.
    // # solve for V
    double volume=(n*R*T)/P;
    testConv(__FILE__,__LINE__,"(volume=(n*R*T)/P == 22.71)",round(volume*100)/100,22.71, errs);
}    

void testGassimConv::testGasConstantCmH20(ErrStrs &errs) 
{    // # the volume of an ideal gas at STP is 22.71 L
    // # PV=nRT
    // # test the value of R against this.
    double P=1019.71621;       // # 1 bar worth of cmH20
    double V=1;      			// # liters
    double R=GassimConst::uGasConstCmH20;
    double n=1;
    double T=GassimConv::cToKelvin(0);   // # 0 degress C.
    // # solve for V
    double volume=(n*R*T)/P;
    testConv(__FILE__,__LINE__,"(volume=(n*R*T)/P == 22.71)",round(volume*100)/100,22.71, errs);
}

bool testGassimConv::main() {
	ErrStrs errs;
	bool rc = false;

	//cout << std::round(100.88888888*100)/100 << endl;
	//return(0);

	// test failure condition
    testConv(__FILE__,__LINE__,"Conv.barToPascals(1)",GassimConv::barToPascals(1),1e5,errs);
    testConv(__FILE__,__LINE__,"Conv.pascalsToBar(1)",GassimConv::pascalsToBar(1),1e-5,errs);
    testConv(__FILE__,__LINE__,"Conv.pascalsToMb(1)",GassimConv::pascalsToMb(1), 1e-2,errs);
    testConv(__FILE__,__LINE__,"round(Conv.cToKelvin(0), 0)",round(GassimConv::cToKelvin(0)), 273,errs);
    testConv(__FILE__,__LINE__,"round(Conv.kevlinToC(273), 0)",round(GassimConv::kevlinToC(273)), 0, errs);
    testConv(__FILE__,__LINE__,"round(Conv.barToCmH2o(1), 2)",round(GassimConv::barToCmH2o(1*100))/100, 1019.72, errs);
    testConv(__FILE__,__LINE__,"round(Conv.cmH2oToBar(1019.71)",round(GassimConv::cmH2oToBar(1019.72)),1, errs);
    testConv(__FILE__,__LINE__,"Conv.lpsToLpm(2)",GassimConv::lpsToLpm(2),120,errs);
    testConv(__FILE__,__LINE__,"Conv.lpmToLps(120)",GassimConv::lpmToLps(120),2,errs);
    testGasConstantBars(errs);
    testGasConstantCmH20(errs);


	if (errs.size() > 0) {
		for (auto iter = errs.begin(); iter != errs.end(); iter++) 
			cout << *iter << endl;
        cout << "TEST Failed" << endl;
        rc = false;
    }
    else {
    	rc = true;
    }

    return(rc);
}


