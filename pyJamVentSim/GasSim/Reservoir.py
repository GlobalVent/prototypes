#
# resovoire with a fixed size...
#

import pprint as pp
from .SimNode import SimNode, NodeType
from . import Const





class Reservoir(SimNode):
    def __init__(self, name, pressure, volume):
        SimNode.__init__(self, name, NodeType.RESEROVIRE);
        self._next_out.pressure = pressure;
        self._next_out.volume = volume;

    def connect(self,node):
        '''
        :param num: -- connection number 0 | 1
        :param node: connection Node
        :return:
        '''
        SimNode.connect(self,node)


    def step(self, dt):
        SimNode.step(self,dt)

        # avoid dealing with both the pressure AND temperature chaning
        #    I am assuming an adbadiac conditions, to avoid a having to
        #      solve 2 ordanary differential equations...

        # keep this simple for each device connected to us for now
        #   assume a single source formula instead of parallel
        #   sources from different pressures...

        # go through the loop and figure out the total mols entering and leaving.
        # then use that to calculate the new pressure
        # and gas mix...
        flowMols=0.0;
        flowMolsN2=0.0;
        flowMolsO2=0.0;
        # for each connection figure out how much is entering or leaving in mols
        R = Const.uGasConst;
        for c in self._connections:
            # convert volume to mol?
            # PV=nRT solve for n
            #    n=PV/RT
            P = c.getPressureDrop(self);
            # print("PresDrop={}".format(P))
            V=c.out.flow*dt;        # convert flow to liters
            T=c.out.temp;
            F=(P*V)/ (R*T);
            # if self.out.pressure >= c.out.pressure:  # outgoing gast
            #     F *= -1;                       # invert the flow
            flowMolsN2+=F*c.out.pN2;
            flowMolsO2+=F*c.out.pO2;
            flowMols+=F;

        # convert resoviore contents to mols.
        P=self.out.pressure;
        V=self.out.volume;
        T=self.out.temp;
        resorvoirMols = (P*V)/(R*T)
        resMolN2 = resorvoirMols * self.out.pN2;
        resMolO2 = resorvoirMols * self.out.pO2;

        n = resorvoirMols + flowMols;
        T = self.out.temp;
        V = self.out.volume;
        self._next_out.pressure = (n * R * T) / V;
        # and now a new gas mix...  convert from Volume
        newMolsN2 = resMolN2 + flowMolsN2;
        newMolsO2 = resMolO2 + flowMolsO2;
        # new mix will what partial pressures will we have.
        self._next_out.pN2 = newMolsN2 / n;
        self._next_out.pO2 = newMolsO2 / n;
        assert (round(self._next_out.pO2 + self._next_out.pN2, 3) == 1.00);
        # this check is unique to this model, nO2 can never be zero or we did
        #  something very wrong.
        assert (self._next_out.pO2 > 0);

    def getValues(self):
        '''
        retrieve the values for this node, and only this node in format ready to do a debug print.
        :return: print string.
        '''
        return("{}: P={:1.6g}, pN2={:1.6g}, pO2={:1.6g}".format(self.getNodeType(),
                                                                self.out.pressure, self.out.pN2, self.out.pO2));













