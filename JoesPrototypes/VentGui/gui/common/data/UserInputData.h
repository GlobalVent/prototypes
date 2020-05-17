#ifndef USERINPUTDATA_H
#define USERINPUTDATA_H

struct UserInputData
{
    float fiO2;
    float tidalVol;
    float respRate;
    int   ie;       // \todo figure out type
    float peep;
    int   ventMode; // \todo figure out type

    UserInputData() : fiO2{21.0}, tidalVol{0.0}, respRate{1.0}, ie{0}, peep{0.0}, ventMode{0} {};
};
#endif // USERINPUTDATA_H


