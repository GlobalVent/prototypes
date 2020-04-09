#!/usr/bin/env python3
# test conversions
#

import sys;
from inspect import currentframe, getframeinfo

from GasSim import Const
from GasSim import Conv


# current file name and linenumbers..
def pyLineNum():
    cf = currentframe()
    return cf.f_back.f_lineno

def pyFileName():
    cf = currentframe()
    return(getframeinfo(cf).filename)

def pyLoc():
    cf = currentframe()
    return("{}: {}".format(getframeinfo(cf).filename,cf.f_back.f_lineno))


def testConv(loc, testStr, v, exp, errs):
    '''
    do converstion test.
    :param loc: location in file of the test.
    :param testStr: test string to display
    :param v: value to check
    :param exp:  expected value
    :param errs: error array to add errors to
    :return:
    '''
    if (v != exp):
        errs.append("(ERROR) {}: {}=={}, exp={}".format(loc,testStr, v, exp))


def testGasConstantBars(errs):
    # the volume of an ideal gas at STP is 22.71 L
    # PV=nRT
    # test the value of R against this.
    P=1;      # bars
    V=1;      # liters
    R=Const.uGasConst;
    n=1;
    T=Conv.cToKelvin(0);   # 0 degress C.
    # solve for V
    volume=(n*R*T)/P;
    testConv(pyLoc(),"(volume=(n*R*T)/P == 22.71)",round(volume,2),22.71, errs);

def testGasConstantCmH20(errs):
    # the volume of an ideal gas at STP is 22.71 L
    # PV=nRT
    # test the value of R against this.
    P=1019.71621;      # 1 bar worth of cmH20
    V=1;      # liters
    R=Const.uGasConstCmH20;
    n=1;
    T=Conv.cToKelvin(0);   # 0 degress C.
    # solve for V
    volume=(n*R*T)/P;
    testConv(pyLoc(),"(volume=(n*R*T)/P == 22.71)",round(volume,2),22.71, errs);


if __name__ == "__main__":

    # test the constants...
    errs= [];
    testConv(pyLoc(),"Conv.barToPascals(1)",Conv.barToPascals(1),1e5,errs)
    testConv(pyLoc(),"Conv.pascalsToBar(1)",Conv.pascalsToBar(1),1e-5,errs)
    testConv(pyLoc(),"Conv.pascalsToMb(1)",Conv.pascalsToMb(1), 1e-2,errs)
    testConv(pyLoc(),"round(Conv.cToKelvin(0), 0)",round(Conv.cToKelvin(0), 0), 273,errs)
    testConv(pyLoc(),"round(Conv.kevlinToC(273), 0)",round(Conv.kevlinToC(273), 0), 0, errs)
    testConv(pyLoc(),"round(Conv.barToCmH2o(1), 2)",round(Conv.barToCmH2o(1), 2), 1019.72, errs)
    testConv(pyLoc(),"round(Conv.cmH2oToBar(1019.71)",round(Conv.cmH2oToBar(1019.72)),1, errs)
    testConv(pyLoc(),"Conv.lpsToLpm(2)",Conv.lpsToLpm(2),120,errs);
    testConv(pyLoc(),"Conv.lpmToLps(120)",Conv.lpmToLps(120),2,errs);
    testGasConstantBars(errs);

    testGasConstantCmH20(errs);

    if (len(errs) > 0):
        print("\n".join(errs));
        print("TEST Failed")
        sys.exit(1)
    else:
        print("TEST Passed");



