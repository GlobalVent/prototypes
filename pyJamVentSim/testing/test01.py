#!/usr/bin/env python3
#
# ultra simple test of basic gas elements.
#
# source -- pipe -- valve -- resovire...

from GasSim.SimNode import SimNode,NodeType
from GasSim.GasSource import GasSource
from GasSim.Valve import Valve
from GasSim.Reservoir import Reservoir
from GasSim import Const
#from GasSim import Conv
#from GasSim import GasSource
#from GasSim import Lungs
#from GasSim import Pipe


class SimpleGasModel(SimNode,):
    def __init__(self):
        SimNode.__init__(self,NodeType.MODEL)
        # create all the nodes...
        #
        gasSrc = GasSource(2)  # 2 bar air source.
        self.addChildNode(gasSrc);
        valveRin = Valve(1,  # pressure
                         Const.reist6mmPipe)  # resistance
        self.addChildNode(valveRin);
        resovoir = Reservoir(1,  # pressure
                             2)  # 2 liter volume
        self.addChildNode(resovoir)
        valveRout = Valve(1,  # pressure
                          Const.resist12mmPipe)  # resistance
        self.addChildNode(valveRout);
        gasSink = GasSource(1)  # 1 bar air sink
        self.addChildNode(gasSink)

        # gasSrc <-> valveRin <-> resovior <-> valveRout <-> gasSink

        # then connect them up..
        gasSrc.connect(valveRin);
        valveRin.connect(gasSrc);

        valveRin.connect(resovoir);
        resovoir.connect(valveRin);

        resovoir.connect(valveRout);
        valveRout.connect(resovoir);

        valveRout.connect(gasSink);
        gasSink.connect(valveRout);





if __name__ == "__main__":
    # keep this very simple
    model = SimpleGasModel();

    dt = 0.001;    # delta time 1 millisecond resolution
    timeLimit = 0.001;  # time limit in seconds.
    timeNow=0;
    while timeNow < timeLimit:
        model.step(dt);
        model.next();
        # todo, extract and display the data...
        timeNow += dt;





