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
# resistance of mm pipe in bar/liters/s
#   This was calculated from some tables on the inernet.
#      and i think they are very wrong...
reist6mmPipe=60;
resist12mmPipe=4.86
reist18mmPipe=1.43
reist24mmPipe=0.619

#
# resistance to get us a desired flow rate.
#   we need to be able to pressurize the container in
#   2 seconds.  for a respiration cycle
#     2 ltr container
resist_5Lps=2;      #0.5 l/s
resist1Lps=1.0;
resist2Lps=0.5
resist4Lps=0.25


#class Conv:


