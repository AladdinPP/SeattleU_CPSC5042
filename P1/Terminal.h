/**
 * @file Terminal.h - class to control a terminal emulator
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */
#pragma once
#include <fstream>
#include <vector>
#include "PixelMatrix.h"

/**
 * @class Terminal - class to control a terminal emulator
 *
 * Uses the ncurses C-library to do this, ugly and finicky as it is.
 */
class Terminal {
public:
    /**
     * Usual constructor. Any objects created actually refer to the exact same terminal underneath.
     *
     * @param blockInGetKey  If false, then use hasKey to see if a key has been pressed before calling
     *                       getKey (else it will throw). If true, then getKey will block until a key is
     *                       typed in the terminal. Note that only the first construction of a Terminal
     *                       will respect this flag--subsequent constructions will ignore it and use the
     *                       value as it was set in the first Terminal object.
     */
    explicit Terminal(bool blockInGetKey = true);

    // big-5 -- any objects created actually refer to the exact same terminal underneath
    ~Terminal();
    Terminal(const Terminal &other);
    Terminal(Terminal &&temp) noexcept;
    Terminal& operator=(const Terminal &other) = default;
    Terminal& operator=(Terminal &&temp) = default;

    /**
     * Get the dimensions of the terminal.
     *
     * @param rowCount  returned by reference the number of rows in the terminal emulator
     * @param colCount  returned by reference the number of columns in the terminal emulator
     */
    void getSize(int &rowCount, int &colCount) const;

    /**
     * Get the number of rows in the terminal.
     *
     * @return number of rows
     */
    int getRowCount() const;

    /**
     * Get the nubmer of columns in the terminal.
     *
     * @return number of columns
     */
    int getColCount() const;

    /**
     * Paint the terminal character cells with the color from the pxm (only those pixels which correspond
     * to valid character cells in the terminal). Pixel colors are given the best match of the available
     * colors. Character cells are "painted" by typing a space character with the color background most closely
     * matching the pixel color.
     *
     * @param pixels  the pixel map with the desired colors for each character cell
     */
    void paint(const PixelMatrix &pixels);

    /**
     * Write some text onto the terminal.  White on black.
     *
     * @param r     row where to start writing
     * @param c     column where to start writing
     * @param text  what to write
     */
    void setText(int r, int c, const std::string &text);

    /**
     * Check if a key has been pressed and is waiting to be read.
     *
     * @return true if getKey will return with a key instantly.
     */
    bool hasKey() const;

    /**
     * Get the last keypress from the terminal.
     * Typically, this is an ASCII letter, like 'q'.
     * Other keys will be returned as values outside the ASCII range
     * (values of these will be added in a future version).
     * If there is no keypress waiting to be read, then the method
     * either blocks, waiting for a key from the terminal (if blockInGetKey,
     * set during construction, is true) or throws (if blockInGetKey is false).
     *
     * @return  the key pressed
     * @throws  logic_error if blockInGetKey is false and there is no key pressed
     */
    int getKey();

    /**
     * Simulate a key being pressed on the terminal. This is pushed onto the
     * beginning of the input queue from the terminal. There is a limit to how
     * many characters can be pushed determined by the details of the terminal.
     *
     * @param c  make this the next character read from getKey
     * @post     next getKey() == c (if no subsequent pushbackKey called)
     */
    void pushbackKey(int c);

    /**
     * Get a list of the colors supported by the terminal emulator.
     *
     * @return  a list of the supported colors
     */
    const std::vector<RGB>& getColors() const;

private:
    struct _Terminal {
        int refcount;
        std::vector<RGB> colors;
    };
    static _Terminal *terminal;  // all the instances of Terminal share this one internal object

    static void init(bool blockInGetKey);
};
