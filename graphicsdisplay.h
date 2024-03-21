#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__

#include <string>
#include "observer.h"
#include "window.h"
#include "piece.h"

class GraphicsDisplay: public Observer {
    Xwindow xw;
    const int dim;
    void drawBlank(const int r, const int c); 
   public:
    GraphicsDisplay(int dim);
    void notify(Piece &piece) override;
    // makes screen an empty grid.
    void clear(); 
};

#endif
