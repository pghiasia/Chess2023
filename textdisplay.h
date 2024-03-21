#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__

#include <vector>
#include <iostream>
#include "observer.h"
#include "piece.h"


class TextDisplay: public Observer {
    std::vector<std::vector<char>> grid; 
    mutable std::string buffer;
    char drawBlank(const int r, const int c);
    void init();
   public:
    TextDisplay();
    void notify(Piece &piece) override;
    void notifyMsg(const std::string &msg);
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
