/**
 * @file Wriggler.h - A Wriggling Worm!
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */

#pragma once
#include "PixelMatrix.h"

/**
 * @class Wriggler - Critter that wriggles around like an inch worm on the screen.
 *
 * Rendered as an 11-pixel pink and red body whose midsection oscillates
 * on each movement. There are four body states that are cycled through as
 * time passes.
 */
class Wriggler {
public:
    Wriggler(int startRow = 10, int startCol = 20, int limitR = 20,
             int limitC = 40) :
    state(A) {
        ROW = startRow;
        COL = startCol;
        limitRow = limitR;
        limitCol = limitC;
    }
    virtual ~Wriggler() {};
    virtual void move();
    virtual void render(PixelMatrix &pxm) const;

    // Getter for row
    int getRow() const {
        return ROW;
    }
    // Getter for column
    int getCol() const {
        return COL;
    }
protected:
    enum State {A, B, C, D};  // four oscillation position states
    State state;
    int ROW, COL;  // Dynamic position variables
    int limitRow, limitCol; // Limits of move range
};

