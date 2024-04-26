/**
 * @author  Hongru He
 * @date    04/19/2024
 * @file    Monster.h
 */

#pragma once
#include "MovingObj.h"

/**
 * Rendered as a monster with eyes and mouth which bounce within a range
 */
class Monster : public MovingObj {
public:
    Monster(int initialRow, int initialCol, int limitR, int limitC) : MovingObj
    (initialRow, initialCol, limitR, limitC) {
        ROW = initialRow;
        COL = initialCol;
        limitRow = limitR;
        limitCol = limitC;
        state = B;
    }
    void render(PixelMatrix &pxm) const override;
};

