#ifndef __LEVEL1_H
#define __LEVEL1_H

#include "computer.h"

class Level1: public Computer {
   public:
    Level1(Colour c);
	Posn genMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
};

class Level2: public Computer {
   public:
    Level2(Colour c);
	Posn genMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
};

class Level3: public Computer {
   public:
    Level3(Colour c);
	Posn genMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
};

class Level4: public Computer {
   public:
    Level4(Colour c);
	Posn genMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
};

#endif
