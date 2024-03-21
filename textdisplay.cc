#include "textdisplay.h"
using namespace std;


TextDisplay::TextDisplay(): grid{vector<vector<char>>(8, vector<char>(8, '_'))} { init(); }

void TextDisplay::init() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            grid[r][c] = drawBlank(r, c);
        }
    }
}

char TextDisplay::drawBlank(const int r, const int c) {
    if (r % 2 == 0) {
        return c % 2 == 0 ? ' ' : '_';
    } else {
        return c % 2 == 0 ? '_' : ' ';
    }
}

void TextDisplay::notify(Piece &piece) {
    const int r = piece.getRow();
    const int c = piece.getCol();
    grid[r][c] = piece.getState() ? piece.identify() : drawBlank(r, c);
}

void TextDisplay::notifyMsg(const string &msg) {
    buffer = msg;
}


ostream &operator<<(ostream &out, const TextDisplay &td) {
    int rowNum = 8;

    out << td.buffer;
    if (td.buffer != "") out << endl;
    td.buffer.clear();

    for (int r = 0; r < 8; ++r) {
        out << rowNum << ' ';
        --rowNum;
        for (int c = 0; c < 8; ++c) {
            out << td.grid[r][c];
        }
        out << endl; 
    }

    out << endl << "  ";
    
    for (char alph = 'a'; alph <= 'h'; ++alph) {
        out << alph; 
    }

    return out << endl << endl; 
}
