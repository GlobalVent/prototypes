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
from GasSim.Reservoir import Reservoir
from GasSim.Pipe import Pipe
from GasSim import Const
#from GasSim import Conv
#from GasSim import GasSource
#from GasSim import Lungs
#from GasSim import Pipe


class SimpleGasModel(SimNode):
    def __init__(self):
        SimNode.__init__(self,"simpleGasModel", NodeType.MODEL)
        # create all the nodes...
        #
        pipeResist=0.1;
        gasSrc = GasSource("gasSrc",3)  #  bar air source.
        self.addChildNode(gasSrc);
        valveRin = Pipe("valveRin", 1,  # pressure
                         pipeResist,  # resistance
                         True);        # open valve
        self.addChildNode(valveRin);
        reservoir = Reservoir("reservoir", 1,  # pressure
                             2)  # 2 liter volume
        self.addChildNode(reservoir)
        valveRout = Pipe("valveRout", 1,  # pressure
                          pipeResist,   # resistance
                         False);        # closed valve.
        self.addChildNode(valveRout);
        gasSink = GasSource("gasSink", 1)  # 1 bar air sink
        self.addChildNode(gasSink)

        self._gasSrc    = gasSrc;
        self._valveRin  = valveRin
        self._reservoir = reservoir
        self._valveRout = valveRout
        self._gasSink   = gasSink;

        #
        # gasSrc <-> valveRin <-> resovior <-> valveRout <-> gasSink
        #  3bar        .1R         1 liter      .1R          1bar
        # pressures are in absolute pressure
        # temperature is 21c
        #   first flow into at step 1 should be 3/.1 = 20
        #    wha??


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

def doModel(dt,timeLimit, stride):
    model = SimpleGasModel();
    gasSrc = model.getGasSrc();
    reservoir = model.getReservoir();
    valveRin = model.getValveRin();
    valveRout = model.getValveRout();
    valveRin.setOpen(True);

    #debug reverse the src and resovire pressures..
    # gasSrc.setPressure(1.0)
    # reservoir.setPressure(3);
    timeNow=0.0;
    nsteps=0;
    T=[];
    Pres=[];
    while timeNow < timeLimit:
        nsteps += 1;
        if ((nsteps % stride) == 0):
            model.step(dt*stride);
            model.next();
        #print("{:1.3}: {}".format(timeNow,reservoir.getValues()))
        if ((nsteps % 200) == 0):
            if (valveRin.out.open):
                valveRin.setOpen(False);
                valveRout.setOpen(True);
            else:
                valveRin.setOpen(True);
                valveRout.setOpen(False);


        timeNow += dt;
        T.append(timeNow);
        Pres.append(reservoir.out.pressure);
    return(T,Pres)

def plotPres(X, Y, color, xMax, yMax):
    plt.plot(X, Y, color=color, linewidth=2.5, linestyle="-")
    plt.xlim(0, xMax)
    #plt.ylim(0,max(reservoir.out.pressure,gasSrc.out.pressure));
    plt.ylim(0, yMax);
    plt.draw();

if __name__ == "__main__":
    # keep this very simple
    dt = 0.01;    # delta time
    timeLimit = 10.0;  # time limit in seconds.

    T=[];       # arrays to plot
    Pres = [];  # reservoir pressures

    plt.axis([0,timeLimit,0,4])
    print("stride=1")
    (T,PresA) = doModel(dt,           # dt
                       timeLimit,     # timelimit
                       1)             # stride

    print("stride=10")
    (T,PresB) = doModel(dt,           # dt
                       timeLimit,     # timelimit
                       20)             # stride

    plotPres(T, PresA, "blue", timeLimit, 4)
    plotPres(T, PresB, "red", timeLimit, 4)
    plt.pause(0.001)        # force it to diplay

    # for a real test, don't do this...
    # if (round(PresA[-1],6) != round(PresB[-1],6)):
    #     print("PresA[-1]({}) != PresB[-1]({})".format(round(PresA[-1],2),round(PresB[-1],2)))
    input("Press [enter] to continue.")
