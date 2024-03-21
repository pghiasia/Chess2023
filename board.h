#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>
#include <sstream>
#include <memory> 
#include <cctype>
#include "level.h"
#include "human.h"
#include "piece.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

class Board {
    std::unique_ptr<Player> p1;
    std::unique_ptr<Player> p2;
    std::unique_ptr<TextDisplay> td;
    std::unique_ptr<GraphicsDisplay> gd;
    // grid is an 8 x 8 vector class, and row 0 in the grid represents the 8th row in the actual chess board.
    // for example: a8 would corespond to row 0, column 0.
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;
    // stores the current players turn.
    Colour currTurn;
    float whiteScore;
    float blackScore;
    // returns a standard chess setup.
    std::vector<std::vector<std::unique_ptr<Piece>>> initSetup(); 
    std::unique_ptr<Player> createPlayer(const std::string &cmd, Colour c); 
    void setup();
    // clears the grid and the displays, and resets the currTurn to the default starter.
    void clear();
    // restarts a new game.
    void restart();
    // takes a string like e1 and converts into row, column numbers.
    void toPosn(const std::string &cmd, int &r, int &c);
    // creates a new Piece based on the cmd given at row r, and column c.
    std::unique_ptr<Piece> pieceFactory(char cmd, int r, int c);
    // verifies that the board contains exactly one white king and exactly one black king; 
    // that no pawns are on the first or last row of the board; and that neither king is in check.
    bool validSetup();
    // returns true if any move, regardless of if making that move would cause a check to that pieces own king,
    // caputres king of colour c.
    bool weakCheck(Colour c);
    // makes move using the coordinate of movePiece.
    // requires: movePiece must have valid move coordinates.
    void makeMove(Player *p); 
    // checks if a pawn did a double move.
    bool pawnDoubleMoved(const Posn &movePiece);
    Posn castleCoordinates(const Posn &movePiece); 
    bool didCastle(const Posn &movePiece); 
    bool didEnPassant(const Posn &movePiece); 
    void setDoubleMoved(const Posn &movePiece);
    bool inStalemate();
    // updates the scoreboard, and the message to be displayed, assuming that a stalemate has occured.
    void stalemateEffects();
    // updates the scoreboard, and the message to be displayed, assuming that a checkmate has occured on player p.
    void checkmateEffects(Player *p); 
    // updates the message to be displayed, assuming that a check has occured.
    void checkEffects(Player *p); 
   public:
    Board(int dim);
    void play();

    friend std::ostream &operator<<(std::ostream &out, const Board &board);
};

#endif
