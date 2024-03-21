#ifndef __COMPUTER_H
#define __COMPUTER_H

#include <random>
#include "player.h"

class Computer: public Player {
   protected:
    // Random num generation
    int randNumGen(int from, int to);
    bool moveAvoidsCapture(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid, 
                           const std::vector<Posn> &enemyMoves, const Posn &movePiece);
    bool isCheckingMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid, const Posn &movePiece);
    bool isCapturingMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid, const Posn &movePiece); 
    int moveEval(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid, const std::vector<Posn> &enemyMoves, const Posn &move);

   public:
    Computer(Colour c);
    virtual ~Computer() = 0;
    std::unique_ptr<Piece> genPromotion(int r, int c) override; 
};

#endif
