#
# valve model.
#
#
# like a pipe but with a control... where the resistance can to to infinity
# or the initial resistance...
# laminar flow only.

from .SimNode import SimNode,NodeType
from .Pipe import Pipe

class Valve(Pipe):
    def __init__(self, pressure, resistance, open=False):
        SimNode.__init__(self,NodeType.VALVE);
        self.out.pressure = pressure;
        self.out.resistance = resistance
        self.out.open = open;

