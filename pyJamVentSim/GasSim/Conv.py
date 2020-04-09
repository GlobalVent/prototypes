

'''
commonly used converstions

NOTE: when porting to C these would become a class with only static methods.
      for python the package file provides the namespace
'''

from . import Const

def barToPascals(bar):
    return(bar * 1e5);
def pascalsToBar(pa):
    return(pa/1e5);

def pascalsToMb(pa):
    return(pa/100);
def mbToPascals(mb):
    return(mb*100);

def cToKelvin(c):
    return(max(0 , c+273.15));

def kevlinToC(k):
    return(max(-273.15, k -273.15));

def barToCmH2o(bar):
    return(Const.cmH20Bar*bar)
def cmH2oToBar(cmH20):
    return(cmH20/Const.cmH20Bar)

def lpmToLps(lpm):      ## liters per minute to liters per second
    return(lpm/60);     # listers per second to liters per minute
def lpsToLpm(lps):
    return(lps*60);


# for now the ideal gas law...
def sccmToccm(sccm,  presPa, tempK):
    '''
    convert sccm -- standard cubic centimeters to ccm (cubic centimeters)
    :param scfm:  standard cubic centimeters per minute
    :param presPa: pressure in pascals.
    :param moles: number of moles
    :param tempK: temperature in kelvin.
    :return: converted ccm (cubic centimeters per minute).
    '''
    return(0);

def ccmToSccm(ccm, presPa, tempK):
    return(0);

