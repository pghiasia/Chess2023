#include "piece.h"
using namespace std;


Piece::Piece(int row, int col, Colour colour, char sym): r{row}, c{col}, colour{colour}, sym{sym}, state{true} {}

void Piece::notifyObservers() {
    for(Observer *o: displays) {
        o->notify(*this);
    }
}

Colour Piece::getColour() const {
    return colour;
}

int Piece::getCol() const {
    return c;
}

int Piece::getRow() const {
    return r;
}

bool Piece::getState() const {
    return state;
}

char Piece::identify() const {
    return sym;
}

void Piece::setEmpty() {
    state = false;
    notifyObservers();
}

void Piece::setReal() {
    state = true;
    notifyObservers();
}

void Piece::setPosn(const Posn &posn) {
    r = posn.r;
    c = posn.c;
}

void Piece::attach(Observer *o) {
   displays.emplace_back(o); 
}



void Piece::addToHistory(const Posn &movePiece) {
    moveHistory.emplace_back(movePiece);
}

bool Piece::isInHistory(const Posn &movePiece) {
    for (auto posn : moveHistory) {
        if (movePiece == posn) {
            return true;
        }
    }

    return false;
}

int Piece::historyLen() {
    return moveHistory.size();
}

bool Piece::getJustDoubleMoved() {
    return false;
}

void Piece::setJustDoubleMoved(bool value) {
    justDoubleMoved = value;
}

bool Pawn::getJustDoubleMoved() {
    return justDoubleMoved;
}


Rook::Rook(int row, int col, Colour c): Piece{row, col, c, c == Colour::White ? 'R' : 'r'} {} 
Knight::Knight(int row, int col, Colour c): Piece{row, col, c, c == Colour::White ? 'N' : 'n'} {} 
Bishop::Bishop(int row, int col, Colour c): Piece{row, col, c, c == Colour::White ? 'B' : 'b'} {} 
Queen::Queen(int row, int col, Colour c): Piece{row, col, c, c == Colour::White ? 'Q' : 'q'} {} 
King::King(int row, int col, Colour c): Piece{row, col, c, c == Colour::White ? 'K' : 'k'} {} 
Pawn::Pawn(int row, int col, Colour c): Piece{row, col, c, c == Colour::White ? 'P' : 'p'} {} 

unique_ptr<Piece> Rook::copy() { return make_unique<Rook>(*this); }
unique_ptr<Piece> Knight::copy() { return make_unique<Knight>(*this); }
unique_ptr<Piece> Bishop::copy() { return make_unique<Bishop>(*this); }
unique_ptr<Piece> Queen::copy() { return make_unique<Queen>(*this); }
unique_ptr<Piece> King::copy() { return make_unique<King>(*this); }
unique_ptr<Piece> Pawn::copy() { return make_unique<Pawn>(*this); }

bool Piece::continueMove(const int row, const int col, vector<Posn> &moves, Piece const *piece) {
    // empty spot
   if (piece == nullptr) { 
       moves.emplace_back(Posn{getRow(), getCol(), row, col});
       return true;
   } 
   // non-empty enemy spot
   if (piece->getColour() != getColour()) {
       moves.emplace_back(Posn{getRow(), getCol(), row, col});
   }

   return false;
}

void Piece::straightMoves(vector<Posn> &moves, const vector<vector<unique_ptr<Piece>>> &grid) {
    int r = getRow();
    int c = getCol();
    
    for(int down = r + 1; down < 8; ++down) {
        if (!continueMove(down, c, moves, grid[down][c].get())) {
            break;
        }
    }

    for(int up = r - 1; up >= 0; --up) {
       if (!continueMove(up, c, moves, grid[up][c].get())) {
           break;
       }
    }

    for(int right = c + 1; right < 8; ++right) {
       if (!continueMove(r, right, moves, grid[r][right].get())) {
           break;
       }
    }

    for(int left = c - 1; left >= 0; --left) {
       if (!continueMove(r, left, moves, grid[r][left].get())) {
           break;
       }
    }

}

void Piece::diagonalMoves(vector<Posn> &moves, const vector<vector<unique_ptr<Piece>>> &grid) {
    int up = getRow() - 1; 
    int down = getRow() + 1; 
    int left = getCol() - 1;
    int right = getCol() + 1;

    while(up >= 0 && left >= 0) {
        if(!continueMove(up, left, moves, grid[up][left].get())) {
            break;
        }
        --up;
        --left;
    }

    up = getRow() - 1; 
    left = getCol() - 1;

    while(down < 8 && left >= 0) {
        if(!continueMove(down, left, moves, grid[down][left].get())) {
            break;
        }
        ++down;
        --left;
    }

    down = getRow() + 1; 
    left = getCol() - 1;

    while(up >= 0 && right <  8) {
        if(!continueMove(up, right, moves, grid[up][right].get())) {
            break;
        }
        --up;
        ++right;
    }

    up = getRow() - 1; 
    right = getCol() + 1;

    while(down < 8 && right < 8) {
        if(!continueMove(down, right, moves, grid[down][right].get())) {
            break;
        }
        ++down;
        ++right;
    }
}

vector<Posn> Piece::filterMoves(const vector<vector<unique_ptr<Piece>>> &grid, const vector<Posn> &allMoves) {
    vector<Posn> moves;
    for (int i = 0; i < allMoves.size(); ++i) {
        if ((0 <= allMoves[i].r && allMoves[i].r < 8) && (0 <= allMoves[i].c && allMoves[i].c < 8)) {
            if (grid[allMoves[i].r][allMoves[i].c] == nullptr) {
                moves.emplace_back(allMoves[i]);
            } else if (grid[allMoves[i].r][allMoves[i].c]->getColour() != colour) {
                moves.emplace_back(allMoves[i]);
            }
        }
    }
    return moves;
}

vector<Posn> Rook::possibleMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    straightMoves(moves, grid);
    return moves;
}

vector<Posn> Bishop::possibleMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    diagonalMoves(moves, grid);
    return moves;
}

vector<Posn> Queen::possibleMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    straightMoves(moves, grid);
    diagonalMoves(moves, grid);
    return moves;
}

vector<Posn> Knight::possibleMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    int r = getRow();
    int c = getCol();

    vector<Posn> allMoves = {Posn{r, c, r + 2, c + 1}, Posn{r, c, r + 2, c - 1}, 
                          Posn{r, c, r - 2, c + 1}, Posn{r, c, r - 2, c - 1},
                          Posn{r, c, r + 1, c + 2}, Posn{r, c, r + 1, c - 2},
                          Posn{r, c, r - 1, c + 2}, Posn{r, c, r - 1, c - 2}};

    return filterMoves(grid, allMoves);
}

bool castleLeft(const vector<vector<unique_ptr<Piece>>> &grid, const int kingR, const int kingC) {
    for (int i = kingC - 1; i >= 1; --i) {
        if (grid[kingR][i] != nullptr) {
            return false;
        }
    }

    // true if left most piece is a rook and both king and rook have made no moves
    return (grid[kingR][0] && tolower(grid[kingR][0]->identify()) == 'r') && grid[kingR][0]->historyLen() == 0
            && grid[kingR][kingC]->historyLen() == 0;
}

bool castleRight(const vector<vector<unique_ptr<Piece>>> &grid, const int kingR, const int kingC) {
    for (int i = kingC + 1; i < 7; ++i) {
        if (grid[kingR][i] != nullptr) {
            return false;
        }
    }

    // true if right most piece is a rook and both king and rook have made no moves
    return (grid[kingR][7] && tolower(grid[kingR][7]->identify()) == 'r') && grid[kingR][7]->historyLen() == 0
            && grid[kingR][kingC]->historyLen() == 0;
}

// Check function which excludes king to avoid infinite recursion
bool inCheck(const vector<vector<unique_ptr<Piece>>> &grid, Colour c) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColour() != c && tolower(grid[i][j]->identify()) != 'k') {
                vector<Posn> moves = grid[i][j]->possibleMoves(grid);
                for (auto posn : moves) {
                    int r = posn.r;
                    int c = posn.c;
                    if (grid[r][c] && tolower(grid[r][c]->identify()) == 'k') {
                        return true;
                    }
                }
            }
        }
    }
    return false; 
}

vector<Posn> King::possibleMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    int r = getRow();
    int c = getCol();
    vector<Posn> moves;

    bool checked = inCheck(grid, getColour());

    vector<Posn> allMoves = {Posn{r, c, r + 1, c}, Posn{r, c, r - 1, c}, Posn{r, c, r, c + 1}, Posn{r, c, r, c - 1},
                          Posn{r, c, r + 1, c + 1}, Posn{r, c, r + 1, c - 1}, Posn{r, c, r - 1, c + 1}, Posn{r, c, r - 1, c - 1}};

    moves = filterMoves(grid, allMoves);
    // Add castling
    if (!checked && castleRight(grid, r, c)) {
        moves.emplace_back(Posn{r, c, r, 6});
    }

    if (!checked && castleLeft(grid, r, c)) {
        moves.emplace_back(Posn{r, c, r, 2});
    }
    
    return moves;
}


vector<Posn> Pawn::possibleMoves(const vector<vector<unique_ptr<Piece>>> &grid) {
    int r = getRow();
    int c = getCol();
    vector<Posn> moves;
    
    const int startRow = getColour() == Colour::White ? 6 : 1;
    const int deltaRow = getColour() == Colour::White ? r - 1 : r + 1;
    const int deltaRow2 = getColour() == Colour::White ? r - 2 : r + 2;

    if (0 <= deltaRow && deltaRow < 8) {
        // Handle moving straight forward
        if (grid[deltaRow][c] == nullptr) { 
            moves.emplace_back(Posn{r, c, deltaRow, c});
        } 
        // Handle attacking diagonally to the right
        if (c + 1 < 8 && grid[deltaRow][c + 1] && grid[deltaRow][c + 1]->getColour() != getColour()) { 
            moves.emplace_back(Posn{r, c, deltaRow, c + 1});
        }
        // Handle attacking diagonally to the left
        if (c - 1 >= 0 && grid[deltaRow][c - 1] && grid[deltaRow][c - 1]->getColour() != getColour()) {
            moves.emplace_back(Posn{r, c, deltaRow, c - 1});
        }
        // If pawn hasn't moved, allow doubled advancement
        if (r == startRow && grid[deltaRow][c] == nullptr && grid[deltaRow2][c] == nullptr) {
            moves.emplace_back(Posn{r, c, deltaRow2, c});
        }
        // Handle en-passant

        // en-passant to the right case
        if (c + 1 < 8 && grid[r][c + 1] && grid[r][c + 1]->getColour() != getColour() && 
            grid[r][c + 1]->getJustDoubleMoved()) {
            moves.emplace_back(Posn{r, c, deltaRow, c + 1});
        }

        // en-passant to the right case
        if (c - 1 >= 0 && grid[r][c - 1] && grid[r][c - 1]->getColour() != getColour() && 
            grid[r][c - 1]->getJustDoubleMoved()) {
            moves.emplace_back(Posn{r, c, deltaRow, c - 1});
        }
    }

    return moves;
}
