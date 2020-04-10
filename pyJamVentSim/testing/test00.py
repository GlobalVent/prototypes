#!/usr/bin/env python3
#
# ultra simple test of basic gas elements.
#
# source -- pipe -- valve -- reservoire...

import sys;
import time;
import pprint as pp
import matplotlib.pyplot as plt


from GasSim.SimNode import SimNode,NodeType
from GasSim.GasSource import GasSource
from GasSim.Valve import Valve
from GasSim.Reservoir import Reservoir
from GasSim import Const


#
# gasSrc <-> valveRin <-> resovior
#  3bar        .1R         1 liter
# pressures are in absolute pressure
# temperature is 21c

# time constant check
# the end result of this after 1 second should be
#    within acceptable rounding errors...
# the same regardless of of dt
#
#

class SimpleGasModel(SimNode):
    def __init__(self,srcPres,resPres):
        SimNode.__init__(self,"simpleGasModel", NodeType.MODEL)
        # create all the nodes...
        #
        pipeResist=0.1;
        gasSrc = GasSource("gasSrc",srcPres)  #  bar air source.
        self.addChildNode(gasSrc);
        valveRin = Valve("valveRin", 1,  # pressure
                         pipeResist)  # resistance
        self.addChildNode(valveRin);
        reservoir = Reservoir("reservoir", resPres,  # pressure
                             2)  # 2 liter volume
        self.addChildNode(reservoir);

        self._gasSrc    = gasSrc;
        self._valveRin  = valveRin
        self._reservoir = reservoir

        # gasSrc <-> valveRin <-> resovior
        # then connect them up..
        gasSrc.connect(valveRin);
        valveRin.connect(gasSrc);

        valveRin.connect(reservoir);
        reservoir.connect(valveRin);

    def getGasSrc(self):
        return(self._gasSrc)
    def getValveRin(self):
        return(self._valveRin);
    def getReservoir(self):
        return(self._reservoir);

# run the model and return
# an array of resovire pressures during the run
#    for plotting and checking...
#
def doModel(dt,timeLimit, stride):
    model = SimpleGasModel(3.0,     # srcPres
                           1.0      # resPres
                           );
    gasSrc = model.getGasSrc();
    reservoir = model.getReservoir();
    valveRin = model.getValveRin();
    valveRin.setOpen(True);

    T=[];       # arrays to plot
    Pres = [];  # reservoir pressures
    nsteps=0;
    timeNow=0.0

    T.append(timeNow);
    Pres.append(reservoir.out.pressure);
    while timeNow < timeLimit:
        nsteps += 1;
        # strid the timesteps, so our number of
        #    samples are the same, so we can plot them
        #    together...
        if ((nsteps % stride) == 0):
            model.step(dt*stride);
            model.next();
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
    timeLimit=4
    plt.axis([0,timeLimit,0,4])
    print("stride=10")
    (T,PresB) = doModel(0.01,          # dt
                       timeLimit,     # timelimit
                       10)             # stride
    print("stride=1")
    (T,PresA) = doModel(0.01,          # dt
                       timeLimit,     # timelimit
                       1)             # stride
    plotPres(T, PresA, "blue", timeLimit, 4)
    plotPres(T, PresB, "red", timeLimit, 4)
    plt.pause(0.001)        # force it to diplay

    # for a real test, don't do this...
    if (round(PresA[-1],6) != round(PresB[-1],6)):
        print("PresA[-1]({}) != PresB[-1]({})".format(round(PresA[-1],2),round(PresB[-1],2)))
    input("Press [enter] to continue.")
