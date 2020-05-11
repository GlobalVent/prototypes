

#include <assert.h>
#include <math.h>
#include <iostream>
#include "GassimLungs.h"


using namespace std;

GassimLungs::GassimLungs(const std::string &name, 
        double pressure, 
        double volume,
        double compliance) : GassimNode(name, LUNGS) {
    setPressure(pressure);
    setVolume(volume);
    setCompliance(compliance);
}
void  GassimLungs::setPO2(double pO2) {
    GassimNode::setPO2(pO2);
};

/**
 * @brief      complianceFactor
 *      compute the compliance factor that can then adjust the
 *      deltaV and deltaP values..
 * @param[in]  compliance  The compliance (0--infinity)
 * @param[in]  deltaV      The delta v
 * @param[in]  deltaP      The delta p
 *
 * @return     multiplcation factor to pass out deltaP to deltaP (0..1)
 */
double GassimLungs::complianceFactor(double compliance, 
                                     double deltaV, 
                                     double deltaP)
{
    //    C*dP
    // ------------
    //  dV + c*dP
    double f = NAN;
    if (compliance == INFINITY)   // INFINITY * 0 is nan, so don't do that...
        f=1;
    else {
        double  dn=deltaV + (compliance * deltaP); // compute the demoniator first, to avoid divide by zero.
        if (dn == 0) 
            f=0;
        else
            f=(compliance*deltaP)/dn;
    }
    return(f);
}


void GassimLungs::step(double dt) {        // # avoid dealing with both the pressure AND temperature chaning

    // # avoid dealing with both the pressure AND temperature chaning
    // #    I am assuming an adbadiac conditions, to avoid a having to
    // #      solve 2 ordanary differential equations...

    // # keep this simple for each device connected to us for now
    // #   assume a single source formula instead of parallel
    // #   sources from different pressures...

    // note: this does not work with two sources of gas
    //       open at once.... (yet)...
    // 
    unsigned numValveOpen=0;
    double deltaP =0;
    double deltaV = 0;
    double P=pressure();
    double V=volume();
    double T=temp();
    double sourcePo2 = 0;
    // this works ONLY for one source at a time.
    //   i don't yet know how to do it for more than one...
    for (auto citer : _connections) {
        NodePtr_t c = citer.second;
        if (! c->open())
            continue;
        numValveOpen += 1;
        // #
        // #                1                # calculate the new pressure..
        // # P      = 1-(-----------)        #   this is only valid for a single
        // #                 dt/RV           #   inlet at a time... no parallel inlets...
        // #               e
        double pDrop = c->getPressureDrop(nodeId());    // pressure drop relative to me...
        double r=c->resistance();                       // lower case r, resistance...
        if (V > 0)
            deltaP = pDrop*(1 - exp(-dt / (r*V)));      
        else
            deltaP = 0;
        deltaV += pDrop*dt;
        sourcePo2+=c->pO2();
    }
    double f=complianceFactor(compliance(), deltaV, deltaP);
    double deltaVadj = deltaV*f;
    double deltaPadj = deltaP*(1-f);

    double newP=pressure()+deltaPadj;              // new totals...
    if (newP < 0)
        newP=0;
    double newV=volume()+deltaVadj;
    if (newV < 0)
        newV = 0;
    double N=0;                        // current N (mols of gas...)
    double No2 = 0;                    // mols of N02
    double newN=0;
    double R=GassimConst::uGasConst;
    if ((V > 0) && (P > 0)) {
        N=P*V/(R*T);          // current moles of gas...
        No2=N*pO2();             
    }
    double deltaNo2=0;     // if either V or P (total are zero, then we have no gas in the container)
    if ((newV > 0) && (newP > 0)) {  // new N total...
        newN=(newP*newV)/(R*T);
        deltaNo2 = ((newN-N)*sourcePo2);
    }
    double newNo2 = No2+deltaNo2;       // now we have a new number of mols of no2
    double newPo2 = newNo2/newN;        // new percent of the mols of gas , no need to convert back 

    setPressure(newP);
    setVolume(newV);
    setPO2(newPo2);
}
