#
# a gas pipe simulation element.
#

import pprint as pp

from .SimNode import SimNode,NodeType



class Pipe(SimNode):
    def __init__(self, name, pressure, resistance, open=True):
        '''
        Pipe (or valve>
        :param name:  name of pipe
        :param pressure:   initial pressure.
        :param resistance: resistance.
        :param open: valve position...
        '''
        SimNode.__init__(self,name, NodeType.PIPE);
        self._next_out.pressure = pressure;
        self._next_out.resistance = resistance;
        self._next_out.open = open;


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
        pA = self._connections[0];
        pB = self._connections[1];
        if (pA.out.pressure > pB.out.pressure):
            maxP = pA;
        else:
            maxP = pB;
        self._next_out.pO2 = maxP.out.pO2;
        self._next_out.pressure = maxP.out.pressure;

        # this needs some refinement,
        #   to get the flow do we need the total resistance?
        if (self.out.open):
            self._next_out.flow=abs(pA.out.pressure-pB.out.pressure)/self.out.resistance;
        else:
            self._next_out.flow = 0;      # valve off, no flow
        return;

    def getPressureDrop(self,node):
        '''
        retrieve the pressure drop,  this number is negative if the pressure
        drop from this node is less than the connecting node.
        :param node: the node requesting the pressure drop
        :return: pressure drop across the node.
        '''
        assert(self.getNumConnections() == 2)
        pdrop = 0;
        if (self.out.open):
            pA = self._connections[0];
            pB = self._connections[1];
            if (node.nodeId() == pB.nodeId()):
                pdrop = (pA.out.pressure-pB.out.pressure)
            else:
                pdrop = (pB.out.pressure-pA.out.pressure)
        else:
            pdrop=0;
        return(pdrop);

    def getResTotal(self,node):
        '''
        getResistance Total.  Total up resistance elements in series.
        :param node: node that is calling this.
        :return: track the resistance chain to both ends..
        '''
        if (self.out.open):
            cA = self._connections[0];
            cB = self._connections[1];
            rTotal=self.out.resistance;   # my own resistance
            if (node.nodeId() != cA.nodeId())
                rTotal+=cA.getResTotal(self);
            if (node.nodeId() != cB.nodeId())
                rTotal+=cB.getResTotal();
        else:
            rTotal=float(inf);
        return(rTotal)  # valve close, infinite resistance.



