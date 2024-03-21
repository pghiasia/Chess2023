#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <string>
class Piece;

class Observer {
   public:
    virtual void notify(Piece &piece) = 0;
    virtual ~Observer() = default;
};

#endif
