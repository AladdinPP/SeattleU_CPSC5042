/**
 * @author  Hongru He
 * @date    04/19/2024
 * @file    Cannon.h
 */

#ifndef P1_CANNON_H
#define P1_CANNON_H

#pragma once
#include "PixelMatrix.h"

/**
 * Rendered as a cannon which stays at the middle bottom of the terminal
 */

class Cannon {
public:
    Cannon(int x, int y) : posX(x), posY(y) {}

    void render(PixelMatrix &pxm) const;

private:
    int posX;
    int posY;
};


#endif //P1_CANNON_H
