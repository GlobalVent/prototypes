#
# resovoire with a fixed size...
#

from .SimNode import SimNode, NodeType


class Reservoir(SimNode):
    def __init__(self, pressure, volume):
        SimNode.__init__(self, NodeType.RESEROVIRE);
        self.setPressure(pressure);
        self.setVolume(volume);

    def connect(self,node):
        '''
        :param num: -- connection number 0 | 1
        :param node: connection Node
        :return:
        '''
        SimNode.connect(self,node)


    def step(self, dt):
        SimNode.step(self,dt)
        print("{} TBD".format(self.getNodeType()));






