/**
 * @author  Hongru He
 * @date    04/19/2024
 * @file    MovingObj.h
 */

#pragma once
#include "PixelMatrix.h"

/**
 * Rendered a basic line object bounce within a range
 */
class MovingObj {
public:
    MovingObj(int startRow = 10, int startCol = 20, int limitR = 20,
              int limitC = 40) :
    state(A) {
        ROW = startRow;
        COL = startCol;
        limitRow = limitR;
        limitCol = limitC;
    }
    virtual ~MovingObj() {};
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

