//
// Created by simon on 2024/4/23.
//

#ifndef P1_CANNON_H
#define P1_CANNON_H

#pragma once
#include "PixelMatrix.h"

class Cannon {
public:
    Cannon(int x, int y) : posX(x), posY(y) {}

    void render(PixelMatrix &pxm) const;

private:
    int posX;
    int posY;
};


#endif //P1_CANNON_H
