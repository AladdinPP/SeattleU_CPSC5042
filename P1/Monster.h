/**
 * @file Wiggler.h - A Wiggling Worm!
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */

#pragma once
#include "Wriggler.h"

/**
 * @class Wiggler - Critter that wiggles around like an inch worm on the screen.
 *
 * Rendered as a 7-pixel green and white body whose midsection oscillates
 * on each movement. There are four body states that are cycled through as
 * time passes.
 */
class Monster : public Wriggler {
public:
    Monster(int initialRow, int initialCol, int limitR, int limitC) : Wriggler
    (initialRow, initialCol, limitR, limitC) {
        ROW = initialRow;
        COL = initialCol;
        limitRow = limitR;
        limitCol = limitC;
        state = B;
    }
    void render(PixelMatrix &pxm) const override;
};

