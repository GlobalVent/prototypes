#ifndef __FMT_STR_H__
#define __FMT_STR_H__
//
//  fmtstr.h
//
//
//  Created by ralph bellofatto on 4/12/20.
//  Copyright © 2020 ralph bellofatto. All rights reserved.
//


#include <iomanip>
#include <ostream>
struct hexw
{
    hexw(unsigned width) : _width(width) {};
    unsigned _width;
};
std::ostream& operator<<(std::ostream& os, const hexw &f) {
    if (f._width)
        os << std::setfill('0');
    else
        os << std::setfill('\0');
    os << std::hex << std::setw(f._width);
    return(os);
}

struct decw
{
    decw(unsigned width) : _width(width) {};
    unsigned _width;
};

std::ostream& operator<<(std::ostream& os, const decw &f) {
    if (f._width)
        os << std::setfill('0');
    else
        os << std::setfill('\0');
    os << std::dec << std::setw(f._width);
    return(os);
}




#endif
