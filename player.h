#ifndef __PLAYER_H
#define __PLAYER_H

#include "piece.h"
#include <memory>

class Player {
    const Colour colour; 
   protected:
    void addVectors(std::vector<Posn> &vec1, const std::vector<Posn> &vec2); 
    // generates and returns all possible enemy moves.
    std::vector<Posn> genEnemyMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid);
    // does a deep copy of a grid.
    std::vector<std::vector<std::unique_ptr<Piece>>> copyGrid(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid);
    // generates what the current grid would look like if that move was made.
    std::vector<std::vector<std::unique_ptr<Piece>>> genFutureGrid(const Posn &movePiece,
                                                                   const std::vector<std::vector<std::unique_ptr<Piece>>> &grid);
    // checks if the move made did not make that player's king in check.
    bool isLegalMove(const Posn &movePiece, const std::vector<std::vector<std::unique_ptr<Piece>>> &grid);
    // Collects all legal moves for the appropriate colour.
    std::vector<Posn> collectLegalMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid); 
   public:  
    Player(Colour c);
    Colour getColour();
    // returns true if player cannot make any legal moves.
    bool noLegalMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid);
    // generates checkmate message, of opponent winning.
    std::string genCheckmateMsg();
    // generates check message.
    std::string genCheckMsg();
    // if colour is equal to player returns 1, otherwise returns 0.
    int getValue(Colour colour); 
    // returns a valid move, if a valid move cannot be returned throws an error.
	virtual Posn genMove(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) = 0;
    // generates the resulting piece that is obtained from a promotion.
    virtual std::unique_ptr<Piece> genPromotion(int r, int c) = 0;
    virtual ~Player() = default;
};

#endif
