#include "computer.h"
#include <limits>
using namespace std;

Computer::Computer(Colour c): Player{c} {}

Computer::~Computer() {}

unique_ptr<Piece> Computer::genPromotion(int r, int c) {
    return make_unique<Queen>(r, c, getColour());
}

int Computer::randNumGen(int from, int to) {
    if (to == -1) {
        return 0;
    }

    int randNum;

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<> dis(from, to);

    randNum = dis(rng); 
    return randNum;
}




bool Computer::moveAvoidsCapture(const vector<vector<unique_ptr<Piece>>> &grid, const vector<Posn> &enemyMoves,
                                 const Posn &movePiece) {
    int r = movePiece.r;
    int c = movePiece.c;
 
    // Check if any enemyMoves target future position of move
    for (int i = 0; i < enemyMoves.size(); ++i) {
        if (enemyMoves[i].r == r && enemyMoves[i].c == c) {
            return false;
        }
    }

    return true;
}

bool Computer::isCapturingMove(const vector<vector<unique_ptr<Piece>>> &grid, const Posn &movePiece) {
    int r = movePiece.r;
    int c = movePiece.c;
    // If not a nullptr piece on grid then must be of opposite colour
    return grid[r][c] != nullptr;
}

bool Computer::isCheckingMove(const vector<vector<unique_ptr<Piece>>> &grid, const Posn &movePiece) {
    int r = movePiece.r;
    int c = movePiece.c;

    // True if target position isn't nullptr and will be in the spot previously held by the king
    return (grid[r][c] && tolower(grid[r][c]->identify()) == 'k');
}


// Return the value of piece being captured
int pieceValue(char piece) {
    if (tolower(piece) == 'p') {
        return 1;
    } else if (tolower(piece) == 'b') {
        return 3;
    } else if (tolower(piece) == 'n') {
        return 3;
    } else if (tolower(piece) == 'r') {
        return 5;
    } else if (tolower(piece) == 'q') {
        return 9;
    } else {
        // King case, return a large number
        return numeric_limits<int>::max();
    }
}


// Returns value of piece killed by a move
int killValue(const vector<vector<unique_ptr<Piece>>> &grid, const Posn &move) {
    int r = move.r;
    int c = move.c;

    // Empty square has no value
    if (grid[r][c] == nullptr) {
        return 0;
    }

    return pieceValue(grid[r][c]->identify());
}


// Returns value of captured piece minus its own value if sacrificed
int Computer::moveEval(const vector<vector<unique_ptr<Piece>>> &grid, const vector<Posn> &enemyMoves, const Posn &move) {
  
    // Initialize to value gained by killing enemy piece
    int moveValue = killValue(grid, move);
  
    // Subtracts value of itself if sacrificed
    if (!moveAvoidsCapture(grid, enemyMoves, move)) {
        moveValue -= pieceValue(grid[move.fromR][move.fromC]->identify());
    }

    return moveValue;
}
