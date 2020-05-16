

#include <assert.h>
#include <math.h>
#include <float.h>
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
 * @brief intersect -- intersect two volume pressure lines given 4 points
 * 
 * @param A point A on line AB
 * @param B point B on line AB
 * @param C point C on line CD
 * @param D point D on line CD
 * @return VP_t 
 */
GassimLungs::VP_t GassimLungs::intersect(VP_t A, VP_t B, VP_t C, VP_t D) {
   // Line AB represented as a1x + b1y = c1
   double a = B.second - A.second;
   double b = A.first - B.first;
   double c = a*(A.first) + b*(A.second);
   // Line CD represented as a2x + b2y = c2
   double a1 = D.second - C.second;
   double b1 = C.first - D.first;
   double c1 = a1*(C.first)+ b1*(C.second);
   double det = a*b1 - a1*b;
   if (det == 0) {
      return make_pair(0, 0);
   } else {
      double x = (b1*c - b*c1)/det;
      double y = (a*c1 - a1*c)/det;
      return make_pair(x, y);
   }
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
    if (V == 0)
        V=DBL_EPSILON;
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
        deltaV += pDrop*(dt/r);
        sourcePo2+=c->pO2();
    }

    if ((deltaV == 0) && (deltaP == 0))
        return;                     // nothing changed so nothing to do.
                                    // the following line intercept won't work for no change

    double deltaVadj, deltaPadj;
    if (compliance() == INFINITY) {   // INFINITY * 0 is nan, so don't do that...
        deltaVadj = deltaV;
        deltaPadj = 0;
    }
    else {
        VP_t p1(deltaV,0);     // line from delta Volume to Delta pressure
        VP_t p2(0, deltaP);
        VP_t p3(compliance(),1);  // complance slope
        VP_t p4(0,0);            // only dealing with the detas so at the origin
        VP_t vp=intersect(p1,p2,p3,p4);
        deltaVadj=vp.first;
        deltaPadj=vp.second;
    }

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
    double newPo2;
    if (newN > 0)
        newPo2 = newNo2/newN;        // new percent of the mols of gas , no need to convert back 
    else
        newPo2=pO2();


    // cout << "newN=" << newN << ", "
    //      << "newV=" << newV << endl;
    // cout << "N=" << N << ", "
    //      << "deltaN=" << newN-N << ", "
    //      << "No2=" << No2 << ", "
    //      << "deltaNo2=" << deltaNo2 << ", "
    //      << "newN=" << newN << ", "
    //      << "newNo2=" << newNo2 << ", "
    //      << "deltaNo2=" << deltaNo2 << ", "
    //      << "newPO2=" << newPo2
    //      << endl;
    setPressure(newP);
    setVolume(newV);
    setPO2(newPo2);
}
