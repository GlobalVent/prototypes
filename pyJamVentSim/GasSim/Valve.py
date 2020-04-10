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
    def __init__(self, name, pressure, resistance, open=False):
        SimNode.__init__(self,name,NodeType.VALVE);
        self._next_out.pressure = pressure;
        self._next_out.resistance = resistance
        self._next_out.open = open;

