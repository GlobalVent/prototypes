#
# container of gas variables.

from . import Const
from . import Conv;
import pprint as pp


# container with gas variables for use in the simulation...
# these will be the inputs, the outputs and the new outputs.
# These are variables of ALL the variables that can be set for any
# given node.

# container of every possible variable we could use...


class GasVars:
    def __init__(self):
        self.pressure = 1.0;           # pressure in the bars
        self.volume   = 1.0;           # current volume. in liters
        self.temp     = Conv.cToKelvin(21);    # room temperature
        self.pO2      = .21;         # percent 02 (partial pressure is p02*pressure)
        self.flow     = 0.0;           # flow...
        self.resistance = 0.0;         # resistance to flow...
        self.open = True;             # if this is a valve, state if it is open or closed...
        self.compliance = 0;            # compliance of 0 is a rigid container...

    # pprint stuff
    def __str__(self):
        return str(pp.pprint(vars(self)))







