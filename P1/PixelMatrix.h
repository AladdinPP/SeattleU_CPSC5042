/**
 * @file PixelMatrix.h - class to store pixel array
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */

#pragma once
#include "RGB.h"

/**
 * @class PixelMatrix - stores a set of pixels for rasterizing.
 */
class PixelMatrix {
public:
    /**
     * Usual constructor sets the beginning dimensions of the pixel matrix.
     *
     * @param nrows  number of rows (y-coordinate in x,y displays)
     * @param ncols  number of columns (x-coordinate in x,y displays)
     * @param color  initial color for all the pixels (default TRANSPARENT)
     */
    PixelMatrix(int nrows, int ncols, const RGB &color = RGB::TRANSPARENT);

    /**
     * Construct from C-style 2d array of RGB
     * @param nrows      number of rows (y-coordinate in x,y displays)
     * @param ncols      number of columns (x-coordinate in x,y displays)
     * @param twod       2d array of RGB of nrows x ncols dimensions
     * @param scaleRow   make each RGB in twod count for this many rows
     * @param scaleCol   make each RGB in twod count for this many columns
     */
    PixelMatrix(int nrows, int ncols, RGB **twod, int scaleRow = 1, int scaleCol = 1);

    /**
     * Constructor that creates a 0x0 matrix (can be resized later).
     */
    PixelMatrix();

    // Rest of the big-5
    ~PixelMatrix();
    PixelMatrix(const PixelMatrix &other);
    PixelMatrix(PixelMatrix &&temp) noexcept;
    PixelMatrix& operator=(const PixelMatrix &other);
    PixelMatrix& operator=(PixelMatrix &&temp) noexcept;

    // Comparison
    bool operator==(const PixelMatrix& other) const;
    bool operator!=(const PixelMatrix& other) const;

    /**
     * Get the current dimensions of the matrix.
     * @param nrows  returned by reference the number of rows in this matrix
     * @param ncols  returned by reference the number of columns in this matrix
     */
    void getSize(int &nrows, int &ncols) const;

    /**
     * Change the dimensions of this matrix.
     * Previous pixel colors are retained if within the new dimensions.
     * New pixels, if any, are assigned the given color.
     * @param nrows  desired number of rows
     * @param ncols  desired number of columns
     * @param color  pixel color for any new pixels
     * @throws       invalid_argument if nrows or ncols less than zero
     * @pre          nrows and ncols non-negative
     * @post         get(r,c) is the same as before for all valid r < old nrows and c < old ncols
     *               get(r,c) == color for valid r >= old nrows and c >= old ncols
     */
    void resize(int nrows, int ncols, const RGB &color = RGB::TRANSPARENT);

    /**
     * Get the pixel color for the given coordinates.
     *
     * @param row  row coordinate
     * @param col  column coordinate
     * @return     the pixel value for (row,column)
     * @throws     out_of_range if row < 0, row >= nrows, col < 0, or col >= ncols
     * @pre        row and col valid
     */
    const RGB& get(int row, int col) const;

    /**
     * Set the pixel color at the given coordinates.
     * Does nothing if row or col are invalid (harmless).
     *
     * @param row     row coordinate
     * @param col     column coordinate
     * @param color   color to set it to
     * @post          if row and col are valid, get(row,col) == color
     */
    void paint(int row, int col, const RGB &color);

    /**
     * Set the pixel colors in the given rectangle to the given pixel color.
     * Pixels within the rectangle but not valid are ignored (harmless).
     * Negative dimension rectangles are ignored (also harmless).
     *
     * @param ulrow  upper-left row coordinate of rectangle to set
     * @param ulcol  upper-left column coordinate of rectangle to set
     * @param lrrow  lower-right row coordinate of rectangle to set
     * @param lrcol  lower-right column coordinate of rectangle to set
     * @param color  color to set all the pixels within the rectangle
     * @post         get(r,c)==color for all valid r,c such
     *               that ulrow <= r <= lrrow and ulcol <= c <= lrcol
     */
    void paint(int ulrow, int ulcol, int lrrow, int lrcol, const RGB &color);

    /**
     * Overlay the non-transparent pixels from another pixel matrix onto this one.
     *
     * @param other  Another pixel matrix whose non-transparent pixels have precedence.
     * @post         For each pixel (r,c) that is a valid spot in this and the other pixel matrix,
     *               after the overlay, get(r,c)==other.get(r,c) unless other.get(r,c).transparent.
     *               nrows and ncols are unchanged
     */
    void overlay(const PixelMatrix &other);

    /**
     * The += operator applies the overlay method.
     *
     * @param other  Another pixel matrix whose non-transparent pixels have precedence.
     * @post         For each pixel (r,c) that is a valid spot in this and the other pixel matrix,
     *               after the overlay, get(r,c)==other.get(r,c) unless other.get(r,c).transparent.
     *               nrows and ncols are unchanged
     */
    PixelMatrix& operator+=(const PixelMatrix& other);

    /**
     * The + operator creates a new pixel matrix where it is a copy of the lhs overlaid with the rhs.
     *
     * @param rhs   the pixel matrix to overlay on this one to create the result
     * @return      a new pixel matrix which is a copy of this and then had the rhs overlaid
     * @post        this
     */
    PixelMatrix operator+(const PixelMatrix& rhs) const;

private:
    int nrows, ncols;  // dimensions of matrix
    RGB **matrix;      // C-style 2D array of RGB structures
};

/**
 * << operator for printing out a pixel matrix
 * @param out output stream to print to
 * @param pxm pixel matrix to print
 * @return    out
 */
std::ostream& operator<<(std::ostream& out, const PixelMatrix& pxm);
