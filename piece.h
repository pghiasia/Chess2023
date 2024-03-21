#ifndef __PIECE_H__
#define __PIECE_H__

#include <vector>
#include <cctype>
#include <memory>
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include "posn.h"
enum class Colour{White, Black};

class Piece {
    int r;
    int c;
    Colour colour;
    char sym;
    bool state;
    std::vector<Observer *> displays;
    std::vector<Posn> moveHistory;
   protected:
    bool justDoubleMoved;

    // if piece is empty continues moving and adds the posn to move (returns true),
    // if piece is an enemy adds it to the possible moves, but can no longer continue moving (returns false).
    bool continueMove(const int row, const int col, std::vector<Posn> &moves, Piece const *p);
    void straightMoves(std::vector<Posn> &moves, const std::vector<std::vector<std::unique_ptr<Piece>>> &grid);
    void diagonalMoves(std::vector<Posn> &moves, const std::vector<std::vector<std::unique_ptr<Piece>>> &grid);
    // filters moves by checking if they are within the 8 by 8 boundaries of a chess board, and
    // if the spot is empty or there is an enemy piece on that spot.
    std::vector<Posn> filterMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid, 
                                  const std::vector<Posn> &allMoves); 
    
   public:
    virtual ~Piece() = default;  
    Piece(int row, int col, Colour colour, char sym);
    void notifyObservers();
    void attach(Observer *o);
    int getRow() const;
    int getCol() const;
    // setEmpty(), changes state to false, and notifies observers that the piece is empty.
    void setEmpty();
    // setReal(), changes state to true, and notifies observers that the piece does exist.
    void setReal();
    bool getState() const;
    void setPosn(const Posn &posn);
    void addToHistory(const Posn &movePiece);
    bool isInHistory(const Posn &movePiece);
    int historyLen();
    // identifies the piece symbol.
    char identify() const;
    Colour getColour() const;
    void setJustDoubleMoved(bool value);

    virtual bool getJustDoubleMoved();

    // returns a list of possibleMoves,including moves that puts that piece's king in check.
    virtual std::vector<Posn> possibleMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) = 0;
    // creates a copy of the piece
    virtual std::unique_ptr<Piece> copy() = 0;
};


class Rook: public Piece {
    char sym;
   public:
    Rook(int row, int col, Colour c);
    std::vector<Posn> possibleMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
    std::unique_ptr<Piece> copy() override; 
};

class Knight: public Piece {
    char sym;
   public:
    Knight(int row, int col, Colour c);
    std::vector<Posn> possibleMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
    std::unique_ptr<Piece> copy() override; 
};

class Bishop: public Piece {
    char sym;
   public:
    Bishop(int row, int col, Colour c);
    std::vector<Posn> possibleMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
    std::unique_ptr<Piece> copy() override; 
};

class Queen: public Piece {
    char sym;
   public:
    Queen(int row, int col, Colour c);
    std::vector<Posn> possibleMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
    std::unique_ptr<Piece> copy() override; 
};

class King: public Piece {
    char sym;
   public:
    King(int row, int col, Colour c);
    std::vector<Posn> possibleMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
    std::unique_ptr<Piece> copy() override; 
};

class Pawn: public Piece {
    char sym;
   public:
    Pawn(int row, int col, Colour c);
    bool getJustDoubleMoved() override;
    std::vector<Posn> possibleMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> &grid) override;
    std::unique_ptr<Piece> copy() override; 
};
#endif
