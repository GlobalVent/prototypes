#
# simulaton model for lungs, variable volume container, with a
#    load of some sort.
# TBD.. how do we represent this?
#

from .SimNode import SimNode,NodeType

class Lungs(SimNode):
    def __init__(self, name, pressure, gasConst, load):
        SimNode.__init__(self,name,NodeType.LUNGS);
        self._next_out.pressure = pressure;
        self._next_out.gasConst = gasConst;
        self._next_out.load = load;       # is this right term.
        print("Lungs.__init__ calculate the volume here from the pressure and load")

    def connect(self, node):
        assert(len(self.__connect)
        SimNode.connect(self,node)
        assert( > 0);        # one connection to a lung..
        connect.append(connect);

    def step(self, dt):
        SimNode.step(self,dt)
        assert(self.getNumConnections() == 2)
        print("{} TBD".format(self.getNodeType()));
