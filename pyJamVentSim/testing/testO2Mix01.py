#!/usr/bin/env python3
#
# test to test the O2 mix.
#
# this run 1/2 the test with
#   pure O2 on the input side.
# and 1/2 the test with pure Air on
#   input side.
#
# see how many cycles it takes to get up to near
#    100% O2
#  and back down to air.
#
#   measure the O2 mix at the resovire
# srcO2 -- pipe -- resovire -- pipe -- srcAir
#           valve      |        valve
#                      |
#                      out
#

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
        self._srcO2 = GasSource("srcO2",3, 1.0)  #  3 bar, 100% O2
        self.addChildNode(self._srcO2);

        self._srcAir = GasSource("srcAir", 3)  # 1 bar air sink
        self.addChildNode(self._srcAir)

        self._gasSink = GasSource("gasSink", 1)  # 1 bar air sink

        self._valveO2In = Pipe("valveO2In", 1,  # pressure
                         pipeResist,  # resistance
                         False);        # open valve

        self.addChildNode(self._valveO2In);

        self._reservoir = Reservoir("reservoir", 1,  # pressure
                             2)  # 2 liter volume
        self.addChildNode(self._reservoir)

        self._valveAirIn = Pipe("valveAirIn", 1,  # pressure
                          pipeResist,   # resistance
                          False);        # closed valve.
        self.addChildNode(self._valveAirIn);

        self._valveGasOut = Pipe("valveGasOut", 1,
                                 pipeResist,
                                 False);
        self.addChildNode(self._valveGasOut)

        #
        # pressures are in absolute pressure
        # temperature is 21c
        #   first flow into at step 1 should be 3/.1 = 20
        #    wha??
        # srcO2 -- valveO2In -- resovire -- valveAirIn -- srcAir
        #                      _gasOut
        self._srcO2.connect(self._valveO2In);
        self._valveO2In.connect(self._reservoir);
        self._srcAir.connect(self._valveAirIn);
        self._valveAirIn.connect(self._reservoir);
        self._reservoir.connect(self._valveGasOut)
        self._valveGasOut.connect(self._gasSink);
        # then connect them up..
    def reservoir(self):
        return(self._reservoir);
    def valveO2In(self):
        return(self._valveO2In);
    def valveAirIn(self):
        return(self._valveAirIn);
    def valveGasOut(self):
        return(self._valveGasOut)

def doModel(dt,timeLimit, stride):
    model = SimpleGasModel();
    valveO2In = model.valveO2In()
    valveAirIn = model.valveAirIn();
    valveGasOut = model.valveGasOut();
    reservoir = model.reservoir();


    valveO2In.setOpen(False)
    valveAirIn.setOpen(True)
    valveGasOut.setOpen(False);

    airValveSeq=[
        # 02,   air,   out
        {"o2In": False, "airIn": True,  "gasOut":False},
        {"o2In": False, "airIn": False, "gasOut":False},
        {"o2In": False, "airIn": False, "gasOut": True},
        {"o2In": False, "airIn": False, "gasOut": False},

    ];
    o2ValveSeq = [
        {"o2In": True, "airIn": False,  "gasOut":False},
        {"o2In": False, "airIn": False, "gasOut":False},
        {"o2In": False, "airIn": False, "gasOut": True},
        {"o2In": False, "airIn": False, "gasOut": False},
    ];
    timeNow=0.0;
    nsteps=0;
    T=[];
    Pres=[];
    pO2=[];


    valveIdx=0;
    #valveSeq = airValveSeq; # initially use the air valve sequence...
    valveSeq = o2ValveSeq; # initially use the air valve sequence...
    while timeNow < timeLimit:
        if ((nsteps % stride) == 0):
            model.step(dt*stride);
            model.next();
        #print("{:1.3}: {}".format(timeNow,reservoir.getValues()))
        if ((round(timeNow,6) == round(timeLimit/2,6))):
            print("switching to Air, nsteps={}".format(nsteps))
            valveSeq=airValveSeq;
        if ((nsteps % 100) == 0):
            valveO2In.setOpen(valveSeq[valveIdx]['o2In'])
            valveAirIn.setOpen(valveSeq[valveIdx]['airIn'])
            valveGasOut.setOpen(valveSeq[valveIdx]['gasOut'])
            valveIdx += 1;
            valveIdx = (valveIdx % len(valveSeq));

        # todo, switch to air sequence
        timeNow += dt;
        T.append(timeNow);
        Pres.append(reservoir.out.pressure);
        pO2.append(reservoir.out.pO2);
        nsteps += 1;

    return(T,Pres,pO2)

def plotMe(p, X, Y, color, xMax, yMax):
    p.plot(X, Y, color=color, linewidth=2.5, linestyle="-")
    p.set_ylim([0, yMax])

    #p.xlim(0, xMax)
    #plt.ylim(0,max(reservoir.out.pressure,gasSrc.out.pressure));
    #p.ylim(0, yMax);
    #p.draw();

if __name__ == "__main__":
    # keep this very simple
    dt = 0.01;    # delta time
    timeLimit = 40.0;  # time limit in seconds.

    T=[];       # arrays to plot
    Pres = [];  # reservoir pressures

    plt.close("all")
    print("stride=1")
    (T,PresA, pO2) = doModel(dt,           # dt
                       timeLimit,     # timelimit
                       1)             # stride
    # print("stride=10")
    # (T,PresB) = doModel(dt,           # dt
    #                    timeLimit,     # timelimit
    #                    20)             # stride
    (f, plots) = plt.subplots(2, sharex=True)
    # presPlt.axis([0,timeLimit,0,4])
    # o2Plt.axis([0, timeLimit, 0, 1])

    plotMe(plots[0], T, PresA, "blue", timeLimit, 4)
    plotMe(plots[1], T, pO2,   "blue", timeLimit, 1)

    # plotPres(T, PresB, "red", timeLimit, 4)
    #plt.pause(0.001)        # force it to diplay
    plt.show();

    # for a real test, don't do this...
    # if (round(PresA[-1],6) != round(PresB[-1],6)):
    #     print("PresA[-1]({}) != PresB[-1]({})".format(round(PresA[-1],2),round(PresB[-1],2)))
    #input("Press [enter] to continue.")
