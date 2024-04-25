/**
 * @file Wriggler.cpp - A Wriggling Worm!
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */

#include "Wriggler.h"

void Wriggler::render(PixelMatrix &pxm) const {
    const int N = 11; // Size of the worm
    const RGB PINK(0xff, 0x00, 0xff);
    const RGB BLOOD(0x8a, 0x03, 0x03);
    //int r = ROW, c = COL;
    //for (int i = 0; i < N; i++) {
    //    // Example logic for rendering horizontally
    //    pxm.paint(r, c++, i % 2 == 0 ? BLOOD : PINK);
    //}

    int col[N], row[N];
    int c = COL;
    for (int i = 0; i < N; i++) {
        col[i] = c++;
        row[i] = ROW;
    }
    for (int i = 0; i < N; i++)
        pxm.paint(row[i], col[i], i % 2 == 0 ? BLOOD : PINK);
}

void Wriggler::move() {
    // Example directional movement logic
    switch(state) {
        case A: // Move right until the right boundary
            COL++;
            if (COL + 11 > limitCol) {
                // switch state to bounce back
                state = C;
                break;
            }
            state = A;
            break;
        case B: // Move down until the bottom boundary
            ROW--;
            if (ROW < 0) {
                // switch state to bounce back
                state = D;
                break;
            };
            state = B;
            break;
        case C: // Move left until the left boundary
            COL--;
            if (COL < 0) {
                // switch state to bounce back
                state = A;
                break;
            }
            state = C;
            break;
        case D: // Move up until the upper boundary
            ROW++;
            if (ROW + 8 > limitRow) {
                // switch state to bounce back
                state = B;
                break;
            }
            state = D;
            break;
    }
}