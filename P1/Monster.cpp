/**
 * @file Wiggler.cpp - A Wiggling Worm!
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */

#include "Monster.h"

void Monster::render(PixelMatrix &pxm) const {
    const int width = 10; // Width of the monster's body
    const int height = 6; // Height of the monster's body
    const RGB GREEN_MONSTER(0, 255, 0); // Main color for the monster
    const RGB BLACK_MOUTH(0, 0, 0); // Color for the mouth
    const RGB WHITE_EYE(255, 255, 255); // Color for the eyes

    // Define the starting position for the monster
    int baseRow = ROW;
    int baseCol = COL;

    // Draw the monster's body (green rectangle)
    for (int r = baseRow; r < baseRow + height; r++) {
        for (int c = baseCol; c < baseCol + width; c++) {
            pxm.paint(r, c, GREEN_MONSTER); // Paint the monster's body
        }
    }

    // Draw the mouth (black horizontal bar)
    int mouthRow = baseRow + height / 2; // Middle of the body
    for (int c = baseCol + 2; c < baseCol + width - 2; c++) {
        pxm.paint(mouthRow, c, BLACK_MOUTH); // Paint the mouth
    }

    // Draw the eyes (white pixels)
    int eyeRow = baseRow + 1; // Position for the eyes
    int leftEyeCol = baseCol + 2;
    int rightEyeCol = baseCol + width - 3;

    pxm.paint(eyeRow, leftEyeCol, WHITE_EYE); // Left eye
    pxm.paint(eyeRow, rightEyeCol, WHITE_EYE); // Right eye
}