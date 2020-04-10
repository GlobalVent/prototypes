#
# gas source with an infinite volume and constant pressure.
#
# there is really no concept of a sink vs a source gas flow can go in
# either direction...

#from . import SimNode
from .SimNode import SimNode,NodeType

class GasSource(SimNode):

    def __init__(self,name,pressure):
        SimNode.__init__(self, name, NodeType.GASSOURCE);
        self._next_out.pressure = pressure;
        self._next_out.volume = float('inf')


    def connect(self, node):
        SimNode.connect(self, node);

    def step(self,dt):
        SimNode.step(self,dt);
        # note nothing changes here, a gas source just
        # is a constant pressure source...
        #


