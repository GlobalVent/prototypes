#
# simulaton model for lungs, variable volume container, with a
#    load of some sort.
# This model of the Lung will have an inhale connection and
# and an exhale connection to keep things simple
# rather than to trace down the connections to see
# which part of a pipe is open.
#

import math
from .SimNode import SimNode,NodeType

class Lungs(SimNode):
    def __init__(self, name, pressure, compliance):
        '''
        Lungs constructure
        :param name: name of node
        :param pressure: initial pressure (usually 1 bar)
        :param compliance: compliance maps to volume.
        '''
        SimNode.__init__(self,name,NodeType.LUNGS);
        self._next_out.pressure = pressure;
        self._next_out.volume=compliance;

    def setCompliance(self, C):
        self._next_out.volume = C;       # compliance is an analog for volume in this situation...
                                    # note we may need a multiplier
    def getLungVolume(self):
        '''
        retrieve the lung volume at 1 atm...
           convert the internal pressure to Atm volume
           using boyles law...  P1*V1=P2*V2
           which solving for V1 becomes
             V1=(P2*V2)/P1
        :return:
        '''
        P1=1;       # convert to 1atm
        P2=self.out.pressure
        V2=self.out.volume;
        V1=(P2*V2)/P1
        return(V1)


    def step(self, dt):
        SimNode.step(self,dt)
        assert(self.getNumConnections() == 2)

        # avoid dealing with both the pressure AND temperature chaning
        #    I am assuming an adbadiac conditions, to avoid a having to
        #      solve 2 ordanary differential equations...

        # keep this simple for each device connected to us for now
        #   assume a single source formula instead of parallel
        #   sources from different pressures...

        # uber simple lung model, RC cirucit analog
        #
        #   ----- R ------
        #                 |
        #                 |
        #                ---
        #                ---    C Compliance (use the volume of the container)
        #                 |
        #                 |
        #              -------
        #                ---
        #                 -
        #
        # we can always make the model more
        #    complicated if we need it.
        numValveOpen=0;
        Pdelta=0;
        ppNewO2=0;
        for c in self._connections:
            if not (c.out.open):
                continue;
            numValveOpen += 1;
            #
            #              1                # calculate the new pressure..
            # P      = (-----------)        #   this is only valid for a single
            #               dt/RV           #   inlet at a time... no parallel inlets...
            #             e
            Pdrop = c.getPressureDrop(self);
            R=c.out.resistance+self.out.resistance;
            V=self.out.volume;
            tc=1-math.exp(-dt/(R*V))
            Pdelta=Pdrop*(1-math.exp(-dt/(R*V)))      # this is not handling multiple circuits open.
            ppNewO2+=Pdelta*c.out.pO2;

        assert(numValveOpen == 1);      # don't support parallel filling of the container yet...
        # ttry working with partial pressures.
        ppO2 = self.out.pressure * self.out.pO2
        ppO2 += ppNewO2;
        self.out.pressure+= Pdelta;
        self.out.pO2=ppO2/self.out.pressure;

        assert(round(self._next_out.pO2,2) <= 1.0);
