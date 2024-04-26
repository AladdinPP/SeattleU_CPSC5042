/**
 * @author  Hongru He
 * @date    04/19/2024
 * @file    CannonBall.h
 */

#ifndef P1_CANNONBALL_H
#define P1_CANNONBALL_H

#pragma once
#include "PixelMatrix.h"

/**
 * Rendered as a cannonball moving upwards and terminated when reach boundary
 */

class CannonBall {
public:
    CannonBall(int startX, int startY, int yLimit)
            : x(startX), y(startY), yLimit(yLimit), isStopped(false) {}

    void move();
    void stop();
    bool isStoppedStatus() const;
    void render(PixelMatrix &pxm) const;
    int getY() const;
    int getX() const;

private:
    int x; // X-coordinate of the cannonball
    int y; // Y-coordinate of the cannonball (moving upwards)
    int yLimit; // Vertical limit
    bool isStopped; // Whether the cannonball is stopped
};


#endif //P1_CANNONBALL_H
