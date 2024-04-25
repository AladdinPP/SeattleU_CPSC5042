/**
 * @file PixelMatrix.cpp - class to store pixel array
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */

#include <stdexcept>
#include "PixelMatrix.h"
using namespace std;

/*
 * This is the typical ctor. Its strategy is to create an empty pxm, then resize it
 */
PixelMatrix::PixelMatrix(int nrows, int ncols, const RGB &color) : PixelMatrix() {
    resize(nrows, ncols, color);
}

PixelMatrix::PixelMatrix(int nrows, int ncols, RGB **twod, int rsc, int csc) : PixelMatrix(nrows*rsc, ncols*csc) {
    for (int r = 0; r < nrows; r++)
        for (int c = 0; c < ncols; c++)
            for (int rsci = 0; rsci < rsc; rsci++)
                for (int csci = 0; csci < csc; csci++)
                    matrix[r*rsc+rsci][c*csc+csci] = twod[r][c];
}

/*
 * This is the zero-arg ctor. Does no allocation. Just sets everything to zeros.
 */
PixelMatrix::PixelMatrix() : nrows(0), ncols(0), matrix(nullptr) {
}

/*
 * Resizing to 0 causes the existing matrix, if any, to be freed.
 */
PixelMatrix::~PixelMatrix() {
    resize(0, 0);
}

/*
 * This is the copy-ctor. By delegating to empty ctor, we are then safe to just use
 * overloaded assignment operator.
 *
 *    PixelMap x = other;
 */
PixelMatrix::PixelMatrix(const PixelMatrix &other) : PixelMatrix() {
    *this = other;
}

/*
 * This is the move-ctor (invoked whenever we are creating a copy from a temporary
 * PixelMatrix that is about to be destroyed). So we can just snarf up its bits.
 * This is the same as the move assignment operator below, so we will use that by
 * first creating an empty with the zero-arg constructor, then assigning.
 *
 *     PixelMap x = y + z
 */
PixelMatrix::PixelMatrix(PixelMatrix &&temp) noexcept : PixelMatrix() {
    *this = temp;
}

/*
 * This is the copy assignment operator. This is invoked when an existing pxm
 * is being assigned into.
 *
 *     x = other
 */
PixelMatrix& PixelMatrix::operator=(const PixelMatrix &other) {
    // only do something if it is not x = x assigning to itself
    if (this != &other) {
        // resize takes care of the memory
        resize(other.nrows, other.ncols);
        // then we copy the pixels
        for (int r = 0; r < nrows; r++)
            for (int c = 0; c < ncols; c++)
                matrix[r][c] = other.matrix[r][c];
    }
    return *this;
}

/*
 * This is the move assignment operator (invoked whenever we are assigning from a temporary
 * PixelMatrix that is about to be destroyed). So we can just snarf up its bits.
 *
 *     x = y + z
 */
PixelMatrix& PixelMatrix::operator=(PixelMatrix &&temp) noexcept {
    swap(matrix, temp.matrix);
    swap(nrows, temp.nrows);
    swap(ncols, temp.ncols);
    return *this;
}

/*
 * Tricky stuff mostly happens in here. We want to preserve any pixels that are still valid,
 * so we have to copy from the old array into the new one. Any new pixels get the default
 * color provided.
 */
void PixelMatrix::resize(int nr, int nc, const RGB &color) {
    if (nr < 0 || nc < 0)
        throw invalid_argument("resize requires nrows >= 0 and ncols >= 0");
    // hold on to the old matrix until we get all the pixels from it that we need in the new one
    RGB **old = matrix;
    if (nr == 0 || nc == 0) {
        matrix = nullptr;
    } else {
        matrix = new RGB*[nr];
        for (int r = 0; r < nr; r++) {
            matrix[r] = new RGB[nc];
            int lastoverlap = r < nrows ? min(nc,ncols) : 0;
            int c;
            for (c = 0; c < lastoverlap; c++)
                matrix[r][c] = old[r][c];
            for (c = lastoverlap; c < nc; c++)
                matrix[r][c] = color;
        }
    }
    // done with the old matrix now, so we can free it
    for (int r = 0; r < nrows; r++)
        delete[] old[r];
    delete[] old;

    nrows = nr;
    ncols = nc;
}

void PixelMatrix::overlay(const PixelMatrix &other) {
    for (int r = 0; r < nrows && r < other.nrows; r++)
        for (int c = 0; c < ncols && c < other.ncols; c++)
            if (!other.matrix[r][c].transparent)
                matrix[r][c] = other.matrix[r][c];
}

const RGB& PixelMatrix::get(int row, int col) const {
    if (row < 0 || row >= nrows || col < 0 || col >= ncols)
        throw out_of_range("no pixel at those coordinates");
    return matrix[row][col];
}

/*
 * Just call the general paint.
 */
void PixelMatrix::paint(int row, int col, const RGB &color) {
    paint(row, col, row, col, color);
}

/*
 * Narrow the rectangle down to only valid pixels, then set them all with a nested loop.
 */
void PixelMatrix::paint(int ulrow, int ulcol, int lrrow, int lrcol, const RGB &color) {
    ulrow = max(0, ulrow);
    ulcol = max(0, ulcol);
    lrrow = min(nrows-1, lrrow);
    lrcol = min(ncols-1, lrcol);
    for (int r = ulrow; r <= lrrow; r++)
        for (int c = ulcol; c <= lrcol; c++)
            matrix[r][c] = color;
}

void PixelMatrix::getSize(int &nrows, int &ncols) const {
    nrows = this->nrows;
    ncols = this->ncols;
}

bool PixelMatrix::operator==(const PixelMatrix& other) const {
    if (nrows != other.nrows || ncols != other.ncols)
        return false;
    for (int r = 0; r < nrows; r++)
        for (int c = 0; c < ncols; c++)
            if (matrix[r][c] != other.matrix[r][c])
                return false;
    return true;
}

/*
 * We don't use !(*this==other) since we can compare for inequality more quickly for small differences
 * by rewriting it here.
 */
bool PixelMatrix::operator!=(const PixelMatrix &other) const {
    if (nrows != other.nrows || ncols != other.ncols)
        return true;
    for (int r = 0; r < nrows; r++)
        for (int c = 0; c < ncols; c++)
            if (matrix[r][c] != other.matrix[r][c])
                return true;
    return false;
}

PixelMatrix& PixelMatrix::operator+=(const PixelMatrix &other) {
    overlay(other);
    return *this;
}

PixelMatrix PixelMatrix::operator+(const PixelMatrix &rhs) const {
    PixelMatrix result = *this;
    result += rhs;
    return result;
}

ostream& operator<<(ostream& out, const PixelMatrix& pxm) {
    int nrows, ncols;
    pxm.getSize(nrows, ncols);
    cout << "(only printing pxm cells that are not BLACK)";
    for (int r = 0; r < nrows; r++) {
        bool printed = false;
        for (int c = 0; c < ncols; c++)
            if (pxm.get(r, c) != RGB::BLACK) {
                if (!printed) {
                    out << endl;
                    out << "\t[" << r << "]: ";
                    printed = true;
                }
                out << "[" << c << "]" << pxm.get(r, c) << " ";
            }
    }
    out << endl;
    return out;
}


