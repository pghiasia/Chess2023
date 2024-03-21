#include "player.h"
using namespace std;

Player::Player(Colour c): colour{c} {}
Colour Player::getColour() { return colour; }

int Player::getValue(Colour colour) {
    return colour == getColour() ? 1 : 0;
}


void Player::addVectors(vector<Posn> &vec1, const vector<Posn> &vec2) {
    for (int i = 0; i < vec2.size(); ++i) {
        vec1.emplace_back(vec2[i]);
    }
}

bool Player::noLegalMoves(const vector<vector<unique_ptr<Piece>>> &grid) { 
   return collectLegalMoves(grid).empty();
}

string Player::genCheckmateMsg() {
   return getColour() == Colour::White ? "Checkmate! Black wins!" : "Checkmate! White wins!";
}

string Player::genCheckMsg() {
    return getColour() == Colour::White ? "White is in check." : "Black is in check.";
}

vector<Posn> Player::collectLegalMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    vector<Posn> new_moves;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColour() == getColour()) {
                new_moves = grid[i][j]->possibleMoves(grid);

                for (int i = 0; i < new_moves.size(); ++i) {
                    if (isLegalMove(new_moves[i], grid)) {
                        moves.emplace_back(new_moves[i]);
                    }
                }
            }
        }
    }

    return moves;
}

vector<Posn> Player::genEnemyMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> enemyMoves;
    vector<Posn> pieceMoves;

    // Collect all legal moves for opposite player
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColour() != getColour()) {
                pieceMoves = grid[i][j]->possibleMoves(grid);
                addVectors(enemyMoves, pieceMoves);
            }
        }
    }

    return enemyMoves;
}


vector<vector<unique_ptr<Piece>>> Player::copyGrid(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<vector<unique_ptr<Piece>>> copy(8);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j]) {
               copy[i].emplace_back(grid[i][j]->copy());
            } else {
                copy[i].emplace_back(nullptr);
            }
        }
    }
    return copy;
}

vector<vector<unique_ptr<Piece>>> Player::genFutureGrid(const Posn &movePiece, const vector<vector<unique_ptr<Piece>>> &grid) {
    auto copy = copyGrid(grid);
    copy[movePiece.fromR][movePiece.fromC]->setPosn(movePiece);
    copy[movePiece.r][movePiece.c] = move(copy[movePiece.fromR][movePiece.fromC]);
    return copy;
}


bool Player::isLegalMove(const Posn &movePiece, const vector<vector<unique_ptr<Piece>>> &grid) {
   auto futureGrid = genFutureGrid(movePiece, grid);
   auto enemyMoves = genEnemyMoves(futureGrid); 
   for (int i = 0; i < enemyMoves.size(); ++i) {
       int r = enemyMoves[i].r;
       int c = enemyMoves[i].c;
       if (futureGrid[r][c] && tolower(futureGrid[r][c]->identify()) == 'k') {
          return false; 
       }
   }
   return true;
}

