#!/usr/bin/env python3
#
# ultra simple test of basic gas elements.
#
# source -- pipe -- valve -- resovire...

import sys;
import pprint as pp
import matplotlib.pyplot as plt


from GasSim.SimNode import SimNode,NodeType
from GasSim.GasSource import GasSource
from GasSim.Valve import Valve
from GasSim.Reservoir import Reservoir
from GasSim import Const
#from GasSim import Conv
#from GasSim import GasSource
#from GasSim import Lungs
#from GasSim import Pipe


class SimpleGasModel(SimNode):
    def __init__(self):
        SimNode.__init__(self,NodeType.MODEL)
        # create all the nodes...
        #
        gasSrc = GasSource(4)  #  bar air source.
        self.addChildNode(gasSrc);
        valveRin = Valve(1,  # pressure
                         Const.reist18mmPipe)  # resistance
        self.addChildNode(valveRin);
        reservoir = Reservoir(1,  # pressure
                             2)  # 2 liter volume
        self.addChildNode(reservoir)
        valveRout = Valve(1,  # pressure
                          Const.resist12mmPipe)  # resistance
        self.addChildNode(valveRout);
        gasSink = GasSource(1)  # 1 bar air sink
        self.addChildNode(gasSink)

        self._gasSrc    = gasSrc;
        self._valveRin  = valveRin
        self._reservoir = reservoir
        self._valveRout = valveRout
        self._gasSink   = gasSink;

        #
        # gasSrc <-> valveRin <-> resovior <-> valveRout <-> gasSink
        #  4bar        18mm         1 liter      18mm          1bar
        # pressures are in absolute pressure
        # temperature is 21c


        # then connect them up..
        gasSrc.connect(valveRin);
        valveRin.connect(gasSrc);

        valveRin.connect(reservoir);
        reservoir.connect(valveRin);

        reservoir.connect(valveRout);
        valveRout.connect(reservoir);

        valveRout.connect(gasSink);
        gasSink.connect(valveRout);

    def getResPressure(self):
        return(self._reservoir)

    def getReservoir(self):
        return(self._reservoir);

    def getValveRin(self):
        return(self._valveRin);
    def getValveRout(self):
        return(self._valveRout);

    def getGasSrc(self):
        return(self._gasSrc)

if __name__ == "__main__":
    # keep this very simple
    model = SimpleGasModel();

    T=[];       # arrays to plot
    Pres = [];  # reservoir pressures

    dt = 0.01;    # delta time 1 millisecond resolution
    timeLimit = 40.0;  # time limit in seconds.
    timeNow=0.0;
    gasSrc = model.getGasSrc();
    reservoir = model.getReservoir();
    valveRin = model.getValveRin();
    valveRout = model.getValveRout();
    valveRin.setOpen(True);

    # debug reverse the src and resovire pressures..
    # gasSrc.setPressure(1.0)
    # reservoir.setPressure(3.999);

    while timeNow < timeLimit:
        model.step(dt);
        model.next();
        print("{:1.3}: {}".format(timeNow,reservoir.getValues()))
        if (round(timeNow,6) == round(timeLimit/4,6)):
            valveRin.setOpen(False);
            valveRout.setOpen(True);

        timeNow += dt;
        T.append(timeNow);
        Pres.append(reservoir.out.pressure);

    # pp.pprint(Pres)
    plt.plot(T, Pres, color="blue", linewidth=2.5, linestyle="-")
    plt.xlim(0, timeLimit)
    #plt.ylim(0,max(reservoir.out.pressure,gasSrc.out.pressure));
    plt.ylim(0,4);
    plt.show()




