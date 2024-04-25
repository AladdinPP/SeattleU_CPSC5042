//
// Created by simon on 2024/4/23.
//

#include "CannonBall.h"

void CannonBall::move() {
    if (!isStopped) {
        y--; // Move upward
        if (y < yLimit) {
            isStopped = true; // Stop if it reaches the vertical limit
        }
    }
}

void CannonBall::stop() {
    isStopped = true; // Manually stop the cannonball
}

bool CannonBall::isStoppedStatus() const {
    return isStopped;
}

void CannonBall::render(PixelMatrix &pxm) const {
    const RGB BALL_COLOR(255, 255, 0); // Yellow color for cannonball
    if (!isStopped) {
        pxm.paint(y, x, BALL_COLOR); // Paint the cannonball if not stopped
    }
}

int CannonBall::getY() const {
    return y;
}

int CannonBall::getX() const {
    return x;
}