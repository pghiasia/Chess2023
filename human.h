#ifndef __HUMAN_H
#define __HUMAN_H

#include <string>
#include "player.h"

class Human: public Player {
    std::unique_ptr<Piece> promotionFactory(char cmd, int r, int c);
   public:
    Human(Colour c);
	Posn genMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
    std::unique_ptr<Piece> genPromotion(int r, int c) override; 
};

#endif
