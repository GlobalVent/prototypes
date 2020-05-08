

#include <assert.h>
#include <math.h>
#include "GassimLungs.h"


GassimLungs::GassimLungs(const std::string &name, 
        double pressure, 
        double volume,
        double compliance) : GassimNode(name, LUNGS) {
    setPressure(pressure);
    setVolume(volume);
    setCompliance(compliance);
}
void  GassimLungs::setPo2(double pO2) {
    assert(pO2 <= 1.0);             // double check...
    assert (pO2 > 0);
    GassimLungs::setPressure(pO2);
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
    double f = 1;
    double  dn=deltaV + (compliance * deltaP); // compute the demoniator first, to avoid divide by zero.
    if (dn != 0) 
        f = (compliance*deltaP)/dn;
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
    double ppNewO2=0;
    for (auto citer : _connections) {
        NodePtr_t c = citer.second;
        if (! c->open())
            continue;
        numValveOpen += 1;
        // #
        // #              1                # calculate the new pressure..
        // # P      = (-----------)        #   this is only valid for a single
        // #               dt/RV           #   inlet at a time... no parallel inlets...
        // #             e
        double pDrop = c->getPressureDrop(nodeId());    // pressure drop relative to me...
        double P=pressure();
        double R=c->resistance();
        double V=volume();
        //double tc=1-exp(-dt/(R*V))
        deltaP = pDrop*(1 - exp(-dt / (R*V)));      //# this is not handling multiple circuits open.
        deltaV = pDrop*(1 - exp(-dt / (R*P)));      //# this is not handling multiple circuits open.
        ppNewO2+=deltaV*(c->pO2());
    }
    double f=complianceFactor(compliance(), deltaV, deltaP);
    double deltaVadj = deltaV*f;
    double deltaPadj = deltaP*(1-f);

    // # try working with partial pressures.
    double ppO2 = pressure() * pO2();
    ppO2 += ppNewO2;
    setPressure(pressure()+deltaPadj);
    setVolume(volume()+deltaVadj);
    setPo2(ppO2/pressure()+deltaP);     // just calculate this based on the full deltaP rather than conver to mols and back.
}
