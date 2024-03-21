#include "graphicsdisplay.h"
using namespace std;

GraphicsDisplay::GraphicsDisplay(int dim): xw{Xwindow(520, 520)}, dim{dim} { clear(); }

void GraphicsDisplay::notify(Piece &piece) {
    const int r = piece.getRow();
    const int c = piece.getCol();
    string font = "-b&h-lucidatypewriter-bold-r-normal-sans-20-140-100-100-m-120-iso8859-10";

    drawBlank(r, c);
    if (piece.getState()) {
      xw.drawString((c * dim) + dim / 2, (r * dim) +  dim / 2, string{piece.identify()}, Xwindow::Red, font);
    } 
}

void GraphicsDisplay::drawBlank(const int r, const int c) {
    if (r % 2 == 0 && c % 2 == 0) {
        xw.fillRectangle(c * dim, r * dim, dim, dim, Xwindow::White); 
    } else if (r % 2 == 0) {
        xw.fillRectangle(c * dim, r * dim, dim, dim, Xwindow::Black); 
    } else if (c % 2 == 0) {
        xw.fillRectangle(c * dim, r * dim, dim, dim, Xwindow::Black); 
    } else {
        xw.fillRectangle(c * dim, r * dim, dim, dim, Xwindow::White); 
    }
}

void GraphicsDisplay::clear() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            drawBlank(r, c);
        }
    }
}

std::ostream &operator<<(std::ostream &out, const GraphicsDisplay &gd);
