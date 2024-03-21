#ifndef __POSN_H__
#define __POSN_H__

#include <compare>

struct Posn {
    int fromR;
    int fromC;
    int r;
    int c;
    auto operator<=>(const Posn &other) const = default;
};

#endif
