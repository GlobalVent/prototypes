# Gas simulation constants and convertions
#

import math;

#
# units for the contstans...
# pressure:   pascals
# flow rate: scm3/s  (standard cubic centimeter per minute)
# radius: cm**2  square cm's
# volume: cm**3  cubic cm's
# gas viscosity: g/cm**3
#

#class Const:
'''
commonly used constants.
'''
pi = math.pi;
# universal gas constant in m3 bar K-1 mol-1
uGasConst=8.3144621e-2;   # gas const in bars/liter
uGasConstCmH20=84.7840;   # gas constant in cmH20/liter


# mol weights for various gases
molWeightAir = 28.9647;        # grams per/mol
molWeightOxygen = 31.999;
molWeightNitrogen = 28.013;
molweightHydrogen = 4.124;

boltzmannsConst = 1.381e-23
avogradoNumber = 6.022e23       # number of molecules in a mol

cmH20Bar = 1019.7162129779;     # how many cm's in 1 bar

# common resistances for our systems.
# resistance of 6mm pipe in bar/liters/s
reist6mmPipe=60;
resist12mmPipe=4.86
reist18mmPipe=1.43
reist24mmPipe=0.619

#class Conv:


