/**
 * @file curses_backend.h
 * @brief Internal (n)curses/PDCurses terminal backend for cmdfx.
 *
 * This is a private header. The rest of the engine talks to the terminal
 * exclusively through this thin abstraction, never to curses directly, so the
 * same code path compiles against ncursesw on POSIX/macOS and PDCurses on
 * Windows.
 *
 * The backend initializes lazily on first use. When stdout is not a terminal
 * (for example under the headless test runner) curses is never started and all
 * operations degrade to safe no-ops: drawing does nothing, the screen size is
 * reported as 0x0, and input polling returns nothing. This keeps the engine and
 * its test suite usable without an attached TTY.
 */
#pragma once

/** Kinds of input event produced by CmdFX_curses_poll. */
typedef enum CmdFX_CursesEventType
{
    CMDFX_CURSES_EVENT_NONE = 0,
    CMDFX_CURSES_EVENT_KEY,
    CMDFX_CURSES_EVENT_MOUSE,
    CMDFX_CURSES_EVENT_RESIZE
} CmdFX_CursesEventType;

/** A single decoded input event from the curses backend. */
typedef struct CmdFX_CursesEvent {
    CmdFX_CursesEventType type;

    // key events
    int keyCode;
    char keyChar;

    // mouse events (coordinates are 1-based, matching the canvas)
    int mouseButton; // 0 left, 1 middle, 2 right, -1 none
    int mousePressed;
    int mouseX;
    int mouseY;

    // resize events
    int width;
    int height;
} CmdFX_CursesEvent;

/**
 * @brief Ensures the curses backend is initialized.
 * @return 1 if a live curses screen is active, 0 if running headless (no TTY).
 */
int CmdFX_curses_ensure();

/** @brief Tears the backend down (endwin) if it was initialized. */
void CmdFX_curses_shutdown();

/** @return 1 when running without an attached terminal. */
int CmdFX_curses_isHeadless();

// Screen

/** @brief Writes the terminal size in cells (0x0 when headless). */
void CmdFX_curses_getSize(int* width, int* height);

// Cursor (coordinates are 1-based to match the cmdfx canvas)

void CmdFX_curses_moveCursor(int x, int y);
void CmdFX_curses_getCursor(int* x, int* y);
void CmdFX_curses_setCursorVisible(int visible);
int CmdFX_curses_isCursorVisible();

// Drawing

void CmdFX_curses_putCharAt(int x, int y, char c);
void CmdFX_curses_putCharHere(char c);

/** @brief Applies an ANSI SGR escape sequence to the current attribute state.
 */
void CmdFX_curses_applySgr(const char* sgr);
void CmdFX_curses_resetAttributes();
void CmdFX_curses_clear();
void CmdFX_curses_refresh();

// Terminal modes

void CmdFX_curses_setEcho(int enabled);
int CmdFX_curses_getEcho();
void CmdFX_curses_setLineBuffered(int enabled);
int CmdFX_curses_getLineBuffered();

// Input

/**
 * @brief Polls one pending input event (non-blocking).
 * @return 1 if an event was written to out, 0 if none was available.
 */
int CmdFX_curses_poll(CmdFX_CursesEvent* out);
