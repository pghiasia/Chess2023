#include "board.h"

using namespace std;

Board::Board(int dim):
    td{unique_ptr<TextDisplay>{new TextDisplay{}}}, 
    gd{unique_ptr<GraphicsDisplay>{new GraphicsDisplay{dim}}}, 
    grid{initSetup()}, currTurn{Colour::White}, whiteScore{0}, blackScore{0} {}

void Board::clear() {
    td = make_unique<TextDisplay>();
    gd->clear();
    currTurn = Colour::White;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            grid[i][j] = nullptr;
        }
    }
}

void Board::restart() {
    clear();
    grid = initSetup();
    p1 = nullptr;
    p2 = nullptr;
}

vector<vector<unique_ptr<Piece>>> Board::initSetup()  {
    vector<vector<unique_ptr<Piece>>> initGrid(8);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            initGrid[i].emplace_back(nullptr);
        }
    }

    initGrid[0][0] = make_unique<Rook>(0, 0, Colour::Black);
    initGrid[0][1] = make_unique<Knight>(0, 1, Colour::Black);
    initGrid[0][2] = make_unique<Bishop>(0, 2, Colour::Black);
    initGrid[0][3] = make_unique<Queen>(0, 3, Colour::Black);
    initGrid[0][4] = make_unique<King>(0, 4, Colour::Black);
    initGrid[0][5] = make_unique<Bishop>(0, 5, Colour::Black);
    initGrid[0][6] = make_unique<Knight>(0, 6, Colour::Black);
    initGrid[0][7] = make_unique<Rook>(0, 7, Colour::Black);
    for(int i = 0; i < 8; ++i) {
        initGrid[1][i] = make_unique<Pawn>(1, i, Colour::Black);
    }

    for (int i = 0; i < 8; ++i) {
        initGrid[6][i] = make_unique<Pawn>(6, i, Colour::White);
    }
    initGrid[7][0] = make_unique<Rook>(7, 0, Colour::White);
    initGrid[7][1] = make_unique<Knight>(7, 1, Colour::White);
    initGrid[7][2] = make_unique<Bishop>(7, 2, Colour::White);
    initGrid[7][3] = make_unique<Queen>(7, 3, Colour::White);
    initGrid[7][4] = make_unique<King>(7, 4, Colour::White);
    initGrid[7][5] = make_unique<Bishop>(7, 5, Colour::White);
    initGrid[7][6] = make_unique<Knight>(7, 6, Colour::White);
    initGrid[7][7] = make_unique<Rook>(7, 7, Colour::White);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (initGrid[i][j] != nullptr) {
                initGrid[i][j]->attach(td.get());
                initGrid[i][j]->attach(gd.get());
                initGrid[i][j]->notifyObservers();
            }
        }
    }
    return initGrid;
}


void Board::toPosn(const string &cmd, int &r, int &c) {
    istringstream iss{cmd};
    char col;
    iss >> col;
    iss >> r;
    r = 8 - r;
    c = col - 'a';
}


unique_ptr<Piece> Board::pieceFactory(char cmd, int r, int c) {
    switch(tolower(cmd)) {
        case 'r':
            return make_unique<Rook>(r, c, cmd == 'R' ? Colour::White : Colour::Black);
        case 'n':
            return make_unique<Knight>(r, c, cmd == 'N' ? Colour::White : Colour::Black);
        case 'b':
            return make_unique<Bishop>(r, c, cmd == 'B' ? Colour::White : Colour::Black);
        case 'q':
            return make_unique<Queen>(r, c, cmd == 'Q' ? Colour::White : Colour::Black);
        case 'k':
            return make_unique<King>(r, c, cmd == 'K' ? Colour::White : Colour::Black);
        case 'p':
            return make_unique<Pawn>(r, c, cmd == 'P' ? Colour::White : Colour::Black);
    }
}

bool Board::validSetup() {
    int whiteKingCounter = 0;
    int blackKingCounter = 0;

    // checks if there are any pawns on the first row or last row.
    for (int i = 0; i < 8; ++i) {
       if (grid[0][i] && tolower(grid[0][i]->identify()) == 'p') {
           return false;
       }
       if (grid[7][i] && tolower(grid[7][i]->identify()) == 'p') {
           return false; 
       }
    }

    // checks that there are no more than one white king and black king.
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->identify() == 'k') {
                ++blackKingCounter;
            } else if (grid[i][j] && grid[i][j]->identify() == 'K') {
                ++whiteKingCounter;
            }
            if (whiteKingCounter > 1 || blackKingCounter > 1) {
                return false;
            }
        }
    }

    return whiteKingCounter == 1 && blackKingCounter == 1 && 
           !weakCheck(Colour::White) && !weakCheck(Colour::Black);
}

bool Board::weakCheck(Colour c) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColour() != c) {
                vector<Posn> moves{grid[i][j]->possibleMoves(grid)};
                for (auto posn: moves) {
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

void Board::setup() {
    clear();
    string cmd;
    int r1 = 0;
    int c1 = 0;
    while(cin >> cmd) {
        if (cmd == "+") {
            char piece;
            string posn;
            cin >> piece >> posn;
            toPosn(posn, r1, c1);
            grid[r1][c1] = pieceFactory(piece, r1, c1);
            grid[r1][c1]->attach(td.get());
            grid[r1][c1]->attach(gd.get());
            grid[r1][c1]->notifyObservers();
            cout << *this;
        } else if (cmd == "-") {
            string posn;
            cin >> posn;
            toPosn(posn, r1, c1);
            if (grid[r1][c1]) {
                grid[r1][c1]->setEmpty();
                grid[r1][c1] = nullptr;
            }
            cout << *this;
        } else if (cmd == "=") {
            string colour;
            cin >> colour;
            currTurn = colour == "White" ||  colour == "white" ? Colour::White : Colour::Black;
        } else if (cmd == "done" && validSetup()) {
            break;
        } else if (cmd == "done") {
            td->notifyMsg("Invalid setup"); 
            cout << *this; 
        }
    }
}

unique_ptr<Player> Board::createPlayer(const string &cmd, Colour c) {
    if (cmd == "human") {
        return unique_ptr<Human>{new Human{c}};
    } else  if (cmd == "computer1")  {
        return unique_ptr<Level1>{new Level1{c}};
    } else if (cmd == "computer2") {
        return unique_ptr<Level2>{new Level2{c}};
    } else if (cmd == "computer3") {
        return unique_ptr<Level3>{new Level3{c}};
    } else if (cmd == "computer4") {
        return unique_ptr<Level4>{new Level4{c}};
    } else {
        return nullptr;
    }
}

bool Board::pawnDoubleMoved(const Posn &movePiece) {
    // double moved if row # changed by 2
    return abs(movePiece.fromR - movePiece.r) == 2;
}

// sets which pieces double moved on grid
void Board::setDoubleMoved(const Posn &movePiece) {
    bool isPawn = (tolower(grid[movePiece.fromR][movePiece.fromC]->identify())) == 'p';
    bool doubleMoved = pawnDoubleMoved(movePiece);

    // Set all old doubledMoved values to false first
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColour() == grid[movePiece.fromR][movePiece.fromC]->getColour()) {
                grid[i][j]->setJustDoubleMoved(false);
            }
        }
    }

    if (isPawn && doubleMoved) {
        grid[movePiece.fromR][movePiece.fromC]->setJustDoubleMoved(true);
    }
}

bool Board::didEnPassant(const Posn &movePiece) {
    bool isPawn = (tolower(grid[movePiece.fromR][movePiece.fromC]->identify())) == 'p';

    // True if pawn attacks an empty square diagonally, thus implying en-passant
    return isPawn && grid[movePiece.r][movePiece.c] == nullptr && movePiece.fromC != movePiece.c;
}

bool Board::didCastle(const Posn &movePiece) {
    return (tolower(grid[movePiece.fromR][movePiece.fromC]->identify()) == 'k' && 
            abs(movePiece.fromC - movePiece.c) == 2);
}

// Returns a Posn with from coordinates as those of Rook position and to coordinates for King position
Posn Board::castleCoordinates(const Posn &movePiece) {
    // left castle case
    if ((movePiece.fromC - movePiece.c) == 2) {
        return Posn{movePiece.fromR, movePiece.fromC - 1, movePiece.r, movePiece.c};
    } else {
    // right castle case
        return Posn{movePiece.fromR, movePiece.fromC + 1, movePiece.r, movePiece.c};
    }
}

void Board::makeMove(Player *p) {
    const Posn movePiece = p->genMove(grid);
    bool isPawn = (tolower(grid[movePiece.fromR][movePiece.fromC]->identify())) == 'p';
    bool isPromotion = isPawn && (movePiece.r == 0 || movePiece.r == 7);
    bool castled = didCastle(movePiece);
    unique_ptr<Piece> tempKing;
    unique_ptr<Piece> tempRook;

    setDoubleMoved(movePiece);

    if (didEnPassant(movePiece)) {
        if (grid[movePiece.fromR][movePiece.fromC]->getColour() == Colour::White) {
            grid[movePiece.r + 1][movePiece.c]->setEmpty();
            grid[movePiece.r + 1][movePiece.c] == nullptr;

        } else {
            grid[movePiece.r - 1][movePiece.c]->setEmpty();
            grid[movePiece.r - 1][movePiece.c] == nullptr;
        }
    }

    // sets old spot to empty, to update display observers.
    grid[movePiece.fromR][movePiece.fromC]->setEmpty();
    
    if (castled) {
        Posn castleCoord = castleCoordinates(movePiece);
        tempKing = move(grid[movePiece.fromR][movePiece.fromC]);
        
        // left castle case
        if ((movePiece.fromC - movePiece.c) == 2) {
            grid[movePiece.r][0]->setEmpty();
            tempRook = move(grid[movePiece.r][0]);
        } else {
        // right castle case
            grid[movePiece.r][7]->setEmpty();
            tempRook = move(grid[movePiece.r][7]);
        }

        grid[castleCoord.fromR][castleCoord.fromC] = move(tempRook);
        grid[castleCoord.r][castleCoord.c] = move(tempKing);

        grid[castleCoord.fromR][castleCoord.fromC]->setPosn(Posn{castleCoord.r, castleCoord.c, castleCoord.fromR, castleCoord.fromC});
        grid[castleCoord.r][castleCoord.c]->setPosn(castleCoord);

        grid[castleCoord.fromR][castleCoord.fromC]->setReal();
        grid[castleCoord.r][castleCoord.c]->setReal();
    }

    if (isPromotion) {
        grid[movePiece.r][movePiece.c] = p->genPromotion(movePiece.r, movePiece.c);
        grid[movePiece.r][movePiece.c]->attach(td.get());
        grid[movePiece.r][movePiece.c]->attach(gd.get());
        grid[movePiece.fromR][movePiece.fromC] = nullptr;
    } else if (!castled) {
        grid[movePiece.fromR][movePiece.fromC]->setPosn(movePiece);
        grid[movePiece.r][movePiece.c] = move(grid[movePiece.fromR][movePiece.fromC]);
    }

    // sets new spot to piece, to update display observers.
    if (!castled) {
        grid[movePiece.r][movePiece.c]->setReal();
    }
}


void Board::stalemateEffects() {
    td->notifyMsg("Stalemate!");
    whiteScore += 0.5;
    blackScore += 0.5;
    cout << *this;
    restart();
}

void Board::checkmateEffects(Player *p) {
    td->notifyMsg(p->genCheckmateMsg());
    whiteScore += p->getValue(Colour::Black);
    blackScore += p->getValue(Colour::White);
    cout << *this;
    restart();
}

void Board::checkEffects(Player *p) {
    td->notifyMsg(p->genCheckMsg());
    cout << *this;
}

void Board::play() {
    string cmd;
    while(cin >> cmd) {
        if (cmd == "game") {
            cin >> cmd;
            p1 = createPlayer(cmd, Colour::White);
            cin >> cmd;
            p2 = createPlayer(cmd, Colour::Black);
            if (p1 && p1->noLegalMoves(grid) && currTurn == p1->getColour()) {
                stalemateEffects();
            } else if (p2 && p2->noLegalMoves(grid) && currTurn == p2->getColour()) {
                stalemateEffects();
            }
        } else if (cmd == "resign") {
            if (currTurn == Colour::White) {
                blackScore += 1;
                td->notifyMsg("Black wins!");
            } else if (currTurn == Colour::Black) {
                whiteScore += 1;
                td->notifyMsg("White wins!");
            }
            cout << *this;
            restart();
        } else if (cmd == "move" && p1 && p2) {
            try {
                if (currTurn == p1->getColour()) {
                    makeMove(p1.get());
                    currTurn = p2->getColour();
                } else {
                    makeMove(p2.get());
                    currTurn = p1->getColour();
                }

                bool p1Check = weakCheck(p1->getColour());
                bool p2Check = weakCheck(p2->getColour());
                bool p1NoLegalMoves = p1->noLegalMoves(grid);
                bool p2NoLegalMoves = p2->noLegalMoves(grid);
                if (p1Check && p1NoLegalMoves) { // p1 has been checkmated
                    checkmateEffects(p1.get());
                } else if (p2Check && p2NoLegalMoves) { // p2 has been checkmated
                    checkmateEffects(p2.get());
                } else if (p1Check || p2Check) { // p1 or p2 are in check
                    checkEffects(p1Check ? p1.get() : p2.get());
                } else if (p1NoLegalMoves && currTurn == p1->getColour()) { // p1 has no legal moves, so stalemate.
                    stalemateEffects();
                } else if (p2NoLegalMoves && currTurn == p2->getColour()) { // p2 has no legal moves, so stalemate.
                    stalemateEffects();
                } else {
                    cout << *this;
                }
            } catch (const std::invalid_argument &error) {
                td->notifyMsg(error.what());
                cout << *this;
            }
        } else if (cmd == "setup") {
            setup();
        } else {
            cout << "INVALID COMMAND: " << cmd << endl; 
        }
    }
    cout << "Final Score:" << endl;
    cout << "White: " << whiteScore << endl;
    cout << "Black: " << blackScore << endl;
}


ostream &operator<<(std::ostream &out, const Board &board) {
    if (board.td) {
        out << *(board.td);
    }
    return out;
}
