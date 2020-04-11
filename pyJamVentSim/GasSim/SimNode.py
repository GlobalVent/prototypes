#
# gas simulation framework
#
# this file contains the gas simulation framework class that the rest
# the simulation is built on
#
# parent child relationship handles a simulation hierachy and
# how the simulation runs.
# most likely there will be one top sim node
#    and everything else are children.
#
# neighbors are all the nodes that are directly connected to a given node.
#   each node is a subclass of the SimNode that knows how to emulate
#      pipe -- a pipe with a constant resistance
#      reservoir -- a fixed volume resoviore.
#      valve -- a valve, with an on/off interface and a constant resistance.
#      lungs -- variable volume resovire with a force that works against it.
#      

from .GasVars import GasVars
from enum import Enum


class NodeType(Enum):
    MODEL='Model'
    GASSOURCE='GasSource'    # for c++ this is likely to be a straight forwared enum.
    LUNGS='Lungs'
    PIPE='Pipe'
    VALVE='Valve'
    RESEROVIRE='Reservoire'

class SimNode:
    __nextId=0;          # simulaton node id, monitically incremented for every new sim node.
    def __init__(self,name,nodeType):
        # public attributes, immutable (please)
        self.out = GasVars();        # public attributes

        # protetected attributes
        self._name=name;
        self._nodeType = nodeType;
        self._next_out = GasVars();       # protected attributes
        self._connections = [];
        self.__childNodes = [];                # ordered array, preservers insertion order
        self.__childSet = set([]);             # set of child nodes to check that if we
                                               # already have a node
        self._nodeId = SimNode.__nextId;
        SimNode.__nextId += 1;
        self.steps=0;

        # should we make an attempt at common inputs and outputs.
        # as an immutable tuple, not all nodes will use these, but...

    def nodeId(self):
        '''
        retrieve the unique node id
        :return:
        '''
        return(self._nodeId)

    def nodeType(self):
        '''
        get the node type of this node...
        :return:
        '''
        return(self._nodeType);

    # virtual classes --
    def step(self, dt):
        '''
        perform the step operation for all the children nodes,
        and then perform the step operation for this node.
        This is where all the magic for each node happens.
        :dt -- detla time for this time step....
        :return: none
        '''
        # some self checks
        if (self.steps == 0):
            self.init();
        assert(self._next_out.pressure > 0);   # should never do 0 or negative pressure
        assert(self._next_out.temp > 0);       # never absolute zero.
        for n in self.__childNodes:
            n.step(dt)

    def init(self):
        self.out = self._next_out;  # initialize initial conditions
        # print("init: {}, resistance={}, open={}".format(self._name,
        #                                                 self.out.resistance,
        #                                                 self.out.open))
        for n in self.__childNodes:
            n.init();

    def name(self):
        return(self._name);
    def next(self):
        '''
        call next in all children node and
        move new data calculated in the step step to the
        :return: none
        '''
        self.steps += 1;
        for n in self.__childNodes:
            n.next()
        # make sure we did not make a mistake with the gas mix...
        self.out = self._next_out;     # move calculated to public output.

    def getNumConnections(self):
        return(len(self._connections));

    def getValues(self):
        '''
        retrieve the values for this node, and only this node in format ready to do a debug print.
        :return: print string.
        '''
        print("getValues -- TBD")

    def getHeader(self):
        '''
        Get a header suitable for use with getValues;
        :return: print string
        '''
        print("getHeader -- TBD")

    def addChildNode(self, node):
        '''
        add a child node to the list of child nodes.
        :param node:
        :return: none
        '''
        assert (not (node.nodeId() in  self.__childSet));
        self.__childNodes.append(node);
        self.__childSet.add(node.nodeId())

    def connect(self, node, connBack=True):
        '''
        connect one node to another...
           all subfunctions must implement this.
           this does a bi directional connection.
           only need to call this on one of the pairs
           and they will both be aware of each other.
        '''
        assert(self.nodeId() != node.nodeId()); # not an unatural act
        assert(not node.nodeId() in self._connections)
        self._connections.append(node);
        if (connBack):                     # don't loop forever back on our selves.
            node.connect(self, connBack=False);     # tell the other guy to cnnect back to me

    # external interface to set values.
    def setPressure(self, pressure):
        self._next_out.pressure = float(pressure);
    def setVolume(self, volume):
        self._next_out.volume = float(volume);
    def setTemperature(self, temp):
        self._next_out.temp = float(temp);
    def setMass(self, mass):
        self._next_out.mass = float(mass);
    def setResistance(self,resistance):
        self._next_out.resistance = float(resistance);
    def setP02(selfself,pO2):
        self._next_out.pO2=float(pO2);
    def setOpen(self, open):
        self._next_out.open = open;

    def setAllTemp(self, temp):
        '''
        set temperature of this and all child nodes.
        :param temp:
        :return:
        '''
        setTemp(temp);
        for c in self.__childNodes:     # pass it on to the children..
            c.setAllTemp(temp);
    #
    # get the pressure drop in a pipe..
    #
    def getPressureDrop(self):
        return(0);      # default is the same as overall pressure.

