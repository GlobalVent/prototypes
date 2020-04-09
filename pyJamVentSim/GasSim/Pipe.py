#
# a gas pipe simulation element.
#

from .SimNode import SimNode,NodeType



class Pipe(SimNode):
    def __init__(self, pressure, resistance):
        SimNode.__init__(self,NodeType.PIPE);
        self.setPressure(pressure);
        self.setResistance(resistance);

    def connect(self, node):
        '''
        :param num: -- connection number 0 | 1
        :param node: connection Node
        :return:
        '''
        assert(self.getNumConnections() <= 2);        # only two connections to a pipe.
        SimNode.connect(self,node);

    def getOtherConnection(self, node):
        '''
        find the node on the other side of this connection
        i.e. the connection that is not the calling node...
        :param node to find the one this is NOT. (uses the node id
        :return: the other node.
        '''
        for n in self._connections:
            if (n.getNodeId() != node.getNodeId()):
                return(n);
        return(None)

    def step(self, dt):
        SimNode.step(self,dt)
        assert(self.getNumConnections() == 2)

        pA=self._connections[0];
        pB=self._connections[1];
        print("{} TBD, {}: pA={}, {}: pB={}".format(self.getNodeType(),
                                                    pA.getNodeType(), pA.out.pressure,
                                                    pB.getNodeType(),pB.out.pressure));






