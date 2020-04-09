#
# gas source with an infinite volume and constant pressure.
#
# there is really no concept of a sink vs a source gas flow can go in
# either direction...

#from . import SimNode
from .SimNode import SimNode,NodeType

class GasSource(SimNode):

    def __init__(self, pressure):
        SimNode.__init__(self, NodeType.GASSOURCE);
        self.out.pressure = pressure;
        self.out.volume = float('inf')


    def connect(self, node):
        SimNode.connect(self, node);

    def step(self,dt):
        SimNode.step(self,dt);
        print("{} TBD".format(self.getNodeType()));
        # note nothing changes here, a gas source just
        # is a constant pressure source...
        #


