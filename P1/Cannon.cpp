/**
 * @author  Hongru He
 * @date    04/19/2024
 * @file    Cannon.cpp
 */

#include "Cannon.h"

// EC: DESIGNED RENDER TO DISTINGUISH OBJECTS AND MAKE THE OBJECT
// UNDERSTANDABLE VISUALLY
void Cannon::render(PixelMatrix &pxm) const {
    const RGB CANNON_COLOR(100, 100, 100);
    const RGB BUMP_COLOR(150, 150, 150);

    for (int i = -2; i <= 2; i++) {
        pxm.paint(posY, posX + i, CANNON_COLOR);
    }

    pxm.paint(posY - 1, posX, BUMP_COLOR);
}