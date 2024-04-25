/**
 * @file RGB.h - class for holding rgb color values
 * @author Kevin Lundeen
 * @see "Seattle Univeristy, CPSC 5042, Spring 2024"
 */

#pragma once
#include <iostream>
#include <vector>

/**
 * @class RGB - class for holding rgb color values
 *
 * Uses a simple struct since everything, including the data members, are public.
 * This is mainly for passing around the 3-byte RGB values, but also includes
 * a transparent bit, se we can indicate when a pixel is transparent. It also has
 * a method for finding best matching color useful for small color depth environments.
 */
struct RGB {
    /**
     * Number of distinct saturation levels for Red, Green, or Blue values.
     * (Number of possible values that fit in a byte.)
     */
    static const int LEVELS = 256;

    /**
     * Some standard fully-saturated colors. Available as pre-constructed objects.
     */
    static const RGB TRANSPARENT, BLACK, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA;

    /**
     * No-arg constructor produces a transparent RGB.
     */
    RGB();

    /**
     * Constructor that takes the r, g, b values
     * @param red    red saturation, 0...LEVEL-1
     * @param green  green saturation, 0...LEVEL-1
     * @param blue   blue saturation, 0...LEVEL-1
     */
    RGB(unsigned char red, unsigned char green, unsigned char blue);

    /**
     * Find the best color match for this color from a set of acceptable colors.
     *
     * Uses naive RGB-space distance. No attempt to use the more accurate HSV-closeness or human LAB model.
     *
     * @param setcolors   list of acceptable RGB colors
     * @return            index into setcolors for the best matching color
     */
    int bestMatch(const std::vector<RGB> &setcolors) const;

    // Equality
    bool operator==(RGB other) const { return transparent == other.transparent && red == other.red && green == other.green && blue == other.blue; }
    bool operator!=(RGB other) const { return !(*this == other); }

    /**
     * The transparent bit is true if the pixel is completely transparent (so RGB values should be ignored)
     */
    bool transparent;

    /**
     * The actual rgb levels. The raison d'etre of this class.
     */
    unsigned char red, green, blue;
};

/**
 * Printing overloaded << operator for RBG objects.  Printed like this: (r148,g97,b0)
 * @param out  the output stream to print to
 * @param rgb  the RGB object to print
 * @return     the output stream for << chaining
 */
std::ostream& operator<<(std::ostream &out, RGB rgb);