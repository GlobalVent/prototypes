#
# container of gas variables.

from . import Const
from . import Conv;


# container with gas variables for use in the simulation...
# these will be the inputs, the outputs and the new outputs.
# These are variables of ALL the variables that can be set for any
# given node.

# container of every possible variable we could use...


class GasVars:
    def __init__(self):
        self.pressure = 1;           # pressure in the bars
        self.volume   = 1;           # current volume. in liters
        self.temp     = Conv.cToKelvin(21);    # room temperature
        self.pO2      = .21;         # percent 02 (partial pressure is p02*pressure)
        self.pN2      = .79;         # percent N2 (partial pressure is p02*pressure)
        self.sccm = 0;               # flow in standard cubic centimeters per minute
        self.resitance = 0;          # resistance to flow...
        self.open = True;            # if this is a valve, state if it is open or closed...

