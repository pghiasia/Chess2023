#include "human.h"
using namespace std;

Human::Human(Colour c): Player{c} {}
Posn Human::genMove(const vector<vector<unique_ptr<Piece>>> &grid) {
    string from;
    string to;
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    cin >> from >> to;

    // Convert string into grid coordinates
    fromCol = from[0] - 'a';
    fromRow = 8 - (from[1] - '0');
    toCol = to[0] - 'a';
    toRow = 8 - (to[1] - '0');

    Posn movePiece = Posn{fromRow, fromCol, toRow, toCol};

    vector<Posn> moves;

    // Adds all possible moves from inputted piece, 
    if (grid[fromRow][fromCol] != nullptr && grid[fromRow][fromCol]->getColour() == getColour()) {
        moves = grid[fromRow][fromCol]->possibleMoves(grid);
    }

    // If input move in legal moves list then play it
    for (int i = 0; i < moves.size(); ++i) {
        if ((moves[i] == movePiece) && isLegalMove(movePiece, grid)) { 
            grid[movePiece.fromR][movePiece.fromC]->addToHistory(movePiece);
            return movePiece;
        } 
    }

    // Throws error if no legal move has been picked
    throw invalid_argument("invalid move");
}

unique_ptr<Piece> Human::genPromotion(int r, int c) {
    char cmd;
    cin >> cmd;
    promotionFactory(cmd, r, c);
}

unique_ptr<Piece> Human::promotionFactory(char cmd, int r, int c) {
    switch(tolower(cmd)) {
        case 'r':
            return make_unique<Rook>(r, c,  getColour());
        case 'n':
            return make_unique<Knight>(r, c, getColour());
        case 'b':
            return make_unique<Bishop>(r, c, getColour());
        case 'q':
            return make_unique<Queen>(r, c, getColour());
    }
    return make_unique<Queen>(r, c, getColour());
}
