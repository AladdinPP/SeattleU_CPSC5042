/**
 * @file Terminal.h - class to control a terminal emulator
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 */

#include <curses.h>
#include <algorithm>
#include "Terminal.h"
using namespace std;

Terminal::_Terminal *Terminal::terminal = nullptr;

void Terminal::init(bool blockInGetKey) {
    if (terminal != nullptr)
        return;
    initscr();
    if (!has_colors()) {
        endwin();
        throw logic_error("terminal does not support colors");
    }
    terminal = new _Terminal;
    terminal->refcount = 0;
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, false);
    keypad(stdscr, true);
    if (!blockInGetKey)
        nodelay(stdscr, true);
    start_color();
    double scale = 255.0/1000.0; // curses uses 0...1000, we want 0...255
    for (short i = 0; i < COLORS; i++) {
        short red, green, blue;
        color_content(i, &red, &green, &blue);
        terminal->colors.push_back(RGB(static_cast<unsigned char>(red * scale),
                                    static_cast<unsigned char>(green * scale),
                                    static_cast<unsigned char>(blue * scale)));
        init_pair(i, COLOR_WHITE, i);
    }
    clear();
}

const vector<RGB>& Terminal::getColors() const {
    return terminal->colors;
}

Terminal::Terminal(bool blockInGetKey) {
    init(blockInGetKey);
    terminal->refcount++;
}
Terminal::Terminal(const Terminal &other) {
    terminal->refcount++;
}
Terminal::Terminal(Terminal &&temp) noexcept {
    terminal->refcount++;
}
Terminal::~Terminal() {
    terminal->refcount--;
    if (terminal->refcount == 0) {
        endwin();
        delete terminal;
        terminal = nullptr;
    }
}

void Terminal::getSize(int &rowCount, int &colCount) const {
    getmaxyx(stdscr, rowCount, colCount);
}

int Terminal::getRowCount() const {
    return getmaxy(stdscr);
}

int Terminal::getColCount() const {
    return getmaxx(stdscr);
}

void Terminal::paint(const PixelMatrix &pixels) {
    int wnrows, wncols, mnrows, mncols, pnrows, pncols;
    getSize(wnrows, wncols);
    pixels.getSize(mnrows, mncols);
    pnrows = min(wnrows, mnrows);
    pncols = min(wncols, mncols);
    for (int r = 0; r < pnrows; r++)
        for (int c = 0; c < pncols; c++) {
            const RGB &color = pixels.get(r, c);
            if (color.transparent)
                continue;
            int best = color.bestMatch(terminal->colors);
            // various hacks:
            if (best == 0)
                best = 16;
            else if (COLORS > 8 && best < 8)
                best += 8;
            attron(COLOR_PAIR(best));
            mvaddch(r, c, ' ');
            attroff(COLOR_PAIR(best));
        }
    refresh();
}

void Terminal::setText(int r, int c, const string &text) {
    attron(COLOR_PAIR(0));
    mvprintw(r, c, "%s", text.c_str());
    attroff(COLOR_PAIR(0));
}

bool Terminal::hasKey() const {
    int c = getch();
    if (c != ERR) {
        ungetch(c);
        return true;
    } else {
        return false;
    }
}

int Terminal::getKey() {
    int c = getch();
    if (c == ERR)
        throw logic_error("no keypress available");
    return c;
}

void Terminal::pushbackKey(int c) {
    ungetch(c);
}
