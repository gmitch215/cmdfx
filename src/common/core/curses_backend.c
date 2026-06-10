#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curses.h>

#include "common/core/curses_backend.h"

// some attributes are missing on certain curses builds (notably PDCurses); fall
// back to A_NORMAL (a no-op) so the SGR mapping still compiles everywhere
#ifndef A_DIM
    #define A_DIM A_NORMAL
#endif
#ifndef A_BLINK
    #define A_BLINK A_NORMAL
#endif
#ifndef A_INVIS
    #define A_INVIS A_NORMAL
#endif
#ifndef A_ITALIC
    #define A_ITALIC A_NORMAL
#endif

// backend state
static int _initialized = 0;
static int _headless = 0;
static int _cursorVisible = 1;
static int _echo = 1;
static int _lineBuffered = 1;
static int _hasColor = 0;

// current drawing attributes; colors are curses color indices (-1 = default)
static attr_t _curAttr = A_NORMAL;
static int _curFg = -1;
static int _curBg = -1;

// color pair cache (pair 0 is reserved by curses for the default)
#define _MAX_PAIRS 256
static short _pairFg[_MAX_PAIRS];
static short _pairBg[_MAX_PAIRS];
static int _pairCount = 0;

// maps a 24-bit rgb value onto the xterm 256-color cube
static int _rgbTo256(int rgb) {
    if (rgb < 0) return -1;
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;
    int r6 = r * 5 / 255;
    int g6 = g * 5 / 255;
    int b6 = b * 5 / 255;
    return 16 + (36 * r6) + (6 * g6) + b6;
}

// resolves (fg, bg) curses color indices to a cached color pair number
static short _resolvePair(int fg, int bg) {
    if (!_hasColor) return 0;

    for (int i = 0; i < _pairCount; i++)
        if (_pairFg[i] == fg && _pairBg[i] == bg) return (short) (i + 1);

    if (_pairCount >= _MAX_PAIRS) return 0;
    if (_pairCount + 1 >= COLOR_PAIRS) return 0;

    int idx = _pairCount;
    _pairFg[idx] = (short) fg;
    _pairBg[idx] = (short) bg;
    init_pair((short) (idx + 1), (short) fg, (short) bg);
    _pairCount++;
    return (short) (idx + 1);
}

// applies the current attribute/color state for subsequent output
static void _applyCurrent() {
    short pair = _resolvePair(_curFg, _curBg);
    attr_set(_curAttr, pair, NULL);
}

int CmdFX_curses_ensure() {
    if (_initialized) return !_headless;
    _initialized = 1;

    // no tty means no curses; run headless so tests and pipes still work
    if (!isatty(STDOUT_FILENO) || !isatty(STDIN_FILENO)) {
        _headless = 1;
        return 0;
    }

    if (initscr() == NULL) {
        _headless = 1;
        return 0;
    }

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    _echo = 0;
    _lineBuffered = 0;

    if (has_colors()) {
        start_color();
        use_default_colors();
        _hasColor = 1;
    }

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    curs_set(_cursorVisible);

    // always restore the terminal on exit
    atexit(CmdFX_curses_shutdown);

    refresh();
    return 1;
}

void CmdFX_curses_shutdown() {
    if (_initialized && !_headless) endwin();

    _initialized = 0;
    _headless = 0;
    _hasColor = 0;
    _pairCount = 0;
    _curAttr = A_NORMAL;
    _curFg = -1;
    _curBg = -1;
}

int CmdFX_curses_isHeadless() {
    CmdFX_curses_ensure();
    return _headless;
}

void CmdFX_curses_getSize(int* width, int* height) {
    if (!CmdFX_curses_ensure()) {
        if (width) *width = 0;
        if (height) *height = 0;
        return;
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if (width) *width = cols;
    if (height) *height = rows;
}

void CmdFX_curses_moveCursor(int x, int y) {
    if (!CmdFX_curses_ensure()) return;
    if (x < 1) x = 1;
    if (y < 1) y = 1;
    move(y - 1, x - 1);
}

void CmdFX_curses_getCursor(int* x, int* y) {
    if (!CmdFX_curses_ensure()) {
        if (x) *x = 0;
        if (y) *y = 0;
        return;
    }

    int row, col;
    getyx(stdscr, row, col);
    if (x) *x = col + 1;
    if (y) *y = row + 1;
}

void CmdFX_curses_setCursorVisible(int visible) {
    _cursorVisible = visible ? 1 : 0;
    if (CmdFX_curses_ensure()) curs_set(_cursorVisible);
}

int CmdFX_curses_isCursorVisible() {
    return _cursorVisible;
}

void CmdFX_curses_putCharAt(int x, int y, char c) {
    if (!CmdFX_curses_ensure()) return;
    if (x < 1) x = 1;
    if (y < 1) y = 1;
    _applyCurrent();
    mvaddch(y - 1, x - 1, (chtype) (unsigned char) c);
}

void CmdFX_curses_putCharHere(char c) {
    if (!CmdFX_curses_ensure()) return;
    _applyCurrent();
    addch((chtype) (unsigned char) c);
}

void CmdFX_curses_applySgr(const char* sgr) {
    if (!CmdFX_curses_ensure()) return;
    if (sgr == 0) return;

    // collect every numeric parameter across the escape sequence
    int params[32];
    int n = 0;
    for (const char* p = sgr; *p && n < 32;) {
        if (*p >= '0' && *p <= '9') {
            int v = 0;
            while (*p >= '0' && *p <= '9') v = (v * 10) + (*p++ - '0');
            params[n++] = v;
        }
        else
            p++;
    }

    // an empty sequence (\033[m) is a full reset
    if (n == 0) {
        _curAttr = A_NORMAL;
        _curFg = -1;
        _curBg = -1;
        _applyCurrent();
        return;
    }

    for (int i = 0; i < n; i++) {
        int code = params[i];
        switch (code) {
            case 0:
                _curAttr = A_NORMAL;
                _curFg = -1;
                _curBg = -1;
                break;
            case 1: _curAttr |= A_BOLD; break;
            case 2: _curAttr |= A_DIM; break;
            case 3: _curAttr |= A_ITALIC; break;
            case 4: _curAttr |= A_UNDERLINE; break;
            case 5: _curAttr |= A_BLINK; break;
            case 7: _curAttr |= A_REVERSE; break;
            case 8: _curAttr |= A_INVIS; break;
            case 22: _curAttr &= ~(A_BOLD | A_DIM); break;
            case 23: _curAttr &= ~A_ITALIC; break;
            case 24: _curAttr &= ~A_UNDERLINE; break;
            case 25: _curAttr &= ~A_BLINK; break;
            case 27: _curAttr &= ~A_REVERSE; break;
            case 28: _curAttr &= ~A_INVIS; break;
            case 38:
                if (i + 4 < n && params[i + 1] == 2) {
                    _curFg = _rgbTo256(
                        (params[i + 2] << 16) | (params[i + 3] << 8) |
                        params[i + 4]
                    );
                    i += 4;
                }
                else if (i + 2 < n && params[i + 1] == 5) {
                    _curFg = params[i + 2];
                    i += 2;
                }
                break;
            case 48:
                if (i + 4 < n && params[i + 1] == 2) {
                    _curBg = _rgbTo256(
                        (params[i + 2] << 16) | (params[i + 3] << 8) |
                        params[i + 4]
                    );
                    i += 4;
                }
                else if (i + 2 < n && params[i + 1] == 5) {
                    _curBg = params[i + 2];
                    i += 2;
                }
                break;
            default:
                // basic 8/16 color foreground and background
                if (code >= 30 && code <= 37)
                    _curFg = code - 30;
                else if (code >= 40 && code <= 47)
                    _curBg = code - 40;
                else if (code >= 90 && code <= 97)
                    _curFg = code - 90 + 8;
                else if (code >= 100 && code <= 107)
                    _curBg = code - 100 + 8;
                break;
        }
    }

    _applyCurrent();
}

void CmdFX_curses_resetAttributes() {
    _curAttr = A_NORMAL;
    _curFg = -1;
    _curBg = -1;
    if (CmdFX_curses_ensure()) _applyCurrent();
}

void CmdFX_curses_clear() {
    if (!CmdFX_curses_ensure()) return;
    clear();
    refresh();
}

void CmdFX_curses_refresh() {
    if (!CmdFX_curses_ensure()) return;
    refresh();
}

void CmdFX_curses_setEcho(int enabled) {
    _echo = enabled ? 1 : 0;
    if (CmdFX_curses_ensure()) {
        if (enabled)
            echo();
        else
            noecho();
    }
}

int CmdFX_curses_getEcho() {
    return _echo;
}

void CmdFX_curses_setLineBuffered(int enabled) {
    _lineBuffered = enabled ? 1 : 0;
    if (CmdFX_curses_ensure()) {
        if (enabled)
            nocbreak();
        else
            cbreak();
    }
}

int CmdFX_curses_getLineBuffered() {
    return _lineBuffered;
}

int CmdFX_curses_poll(CmdFX_CursesEvent* out) {
    if (out == 0) return 0;
    if (!CmdFX_curses_ensure()) return 0;

    int ch = getch();
    if (ch == ERR) return 0;

    memset(out, 0, sizeof(CmdFX_CursesEvent));
    out->mouseButton = -1;

    if (ch == KEY_RESIZE) {
        out->type = CMDFX_CURSES_EVENT_RESIZE;
        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        out->width = cols;
        out->height = rows;
        return 1;
    }

    if (ch == KEY_MOUSE) {
        MEVENT me;
        // ncurses uses getmouse(MEVENT*); PDCurses' getmouse() takes no args
        // and exposes the ncurses-compatible version as nc_getmouse()
#ifdef PDCURSES
        if (nc_getmouse(&me) != OK) return 0;
#else
        if (getmouse(&me) != OK) return 0;
#endif

        out->type = CMDFX_CURSES_EVENT_MOUSE;
        out->mouseX = me.x + 1;
        out->mouseY = me.y + 1;

        if (me.bstate & (BUTTON1_PRESSED | BUTTON1_CLICKED)) {
            out->mouseButton = 0;
            out->mousePressed = 1;
        }
        else if (me.bstate & BUTTON1_RELEASED) {
            out->mouseButton = 0;
            out->mousePressed = 0;
        }
        else if (me.bstate & (BUTTON2_PRESSED | BUTTON2_CLICKED)) {
            out->mouseButton = 1;
            out->mousePressed = 1;
        }
        else if (me.bstate & (BUTTON3_PRESSED | BUTTON3_CLICKED)) {
            out->mouseButton = 2;
            out->mousePressed = 1;
        }
        return 1;
    }

    out->type = CMDFX_CURSES_EVENT_KEY;
    out->keyCode = ch;
    out->keyChar = (ch >= 0 && ch < 256) ? (char) ch : 0;
    out->mousePressed = 1; // terminals report presses only
    return 1;
}
