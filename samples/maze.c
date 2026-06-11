/**
 * @file maze.c
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @date 2025-06-10
 *
 * This is a simple game where you explore a procedurally generated maze. You
 * control a one-pixel character (`@`) that stays in the middle of the screen
 * while the maze (built from `#` characters) scrolls underneath you using the
 * CmdFX Scenes API.
 *
 * The maze is larger than the terminal, and the scene is scrolled with
 * `Scene_scroll` whenever you move, keeping the player centered. Walls block
 * movement, so the scene only scrolls into open corridors.
 *
 * Controls:
 * - W / A / S / D: move (scrolls the maze)
 * - Q: quit
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <cmdfx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// scene + maze state
static CmdFX_Scene* maze = 0;
static int mazeUid = -1;
static char** mazeData = 0;
static char*** mazeAnsiData = 0;
static int mazeW = 0;
static int mazeH = 0;

// viewport (screen) + camera state
static int viewW = 0;   // visible columns
static int viewH = 0;   // visible rows (bottom row reserved for the hud)
static int camX = 0;    // top-left maze column shown on screen
static int camY = 0;    // top-left maze row shown on screen
static int centerX = 0; // player screen column (1-based)
static int centerY = 0; // player screen row (1-based)

// player position within the maze; stays centered on screen
static int playerX = 0;
static int playerY = 0;

static volatile int running = 1;

static int clampi(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// next odd value at or above n
static int oddify(int n) {
    return (n % 2 == 0) ? n + 1 : n;
}

// carves a perfect maze into mazeData using an iterative recursive backtracker
static void generateMaze(void) {
    // start fully walled
    for (int y = 0; y < mazeH; y++)
        for (int x = 0; x < mazeW; x++) mazeData[y][x] = '#';

    int* stackX = malloc(sizeof(int) * mazeW * mazeH);
    int* stackY = malloc(sizeof(int) * mazeW * mazeH);
    if (stackX == 0 || stackY == 0) {
        free(stackX);
        free(stackY);
        return;
    }

    int dirs[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

    int sp = 0;
    mazeData[1][1] = ' ';
    stackX[sp] = 1;
    stackY[sp] = 1;
    sp++;

    while (sp > 0) {
        int x = stackX[sp - 1];
        int y = stackY[sp - 1];

        // shuffle the four directions
        int order[4] = {0, 1, 2, 3};
        for (int i = 3; i > 0; i--) {
            int j = rand() % (i + 1);
            int t = order[i];
            order[i] = order[j];
            order[j] = t;
        }

        int moved = 0;
        for (int i = 0; i < 4; i++) {
            int d = order[i];
            int nx = x + dirs[d][0];
            int ny = y + dirs[d][1];

            // keep the outer border walled
            if (nx <= 0 || ny <= 0 || nx >= mazeW - 1 || ny >= mazeH - 1)
                continue;
            // skip already-carved cells
            if (mazeData[ny][nx] == ' ') continue;

            // carve the wall between and the neighbor
            mazeData[y + dirs[d][1] / 2][x + dirs[d][0] / 2] = ' ';
            mazeData[ny][nx] = ' ';
            stackX[sp] = nx;
            stackY[sp] = ny;
            sp++;
            moved = 1;
            break;
        }

        if (!moved) sp--; // backtrack
    }

    free(stackX);
    free(stackY);
}

// stamps the player on top of the maze at its current screen position (the
// center, except near the maze edges where the camera can no longer scroll)
static void drawPlayer(void) {
    Canvas_setForeground(0xFFD000);
    Canvas_enableBold();
    Canvas_setChar(playerX - camX + 1, playerY - camY + 1, '@');
    Canvas_resetFormat();
}

int onKeyPress(CmdFX_Event* event) {
    CmdFX_KeyEvent* payload = (CmdFX_KeyEvent*) event->data;
    char c = payload->keyChar;

    if (c == 'q') {
        running = 0;
        return 0;
    }

    int dx = 0, dy = 0;
    if (c == 'w')
        dy = -1;
    else if (c == 's')
        dy = 1;
    else if (c == 'a')
        dx = -1;
    else if (c == 'd')
        dx = 1;
    else
        return 0;

    int nx = playerX + dx;
    int ny = playerY + dy;

    // block out-of-bounds and walls (the player never phases through walls)
    if (nx < 0 || ny < 0 || nx >= mazeW || ny >= mazeH) return 0;
    if (mazeData[ny][nx] == '#') return 0;

    // the player's current screen cell, in case the view does not scroll
    int oldSX = playerX - camX + 1;
    int oldSY = playerY - camY + 1;

    playerX = nx;
    playerY = ny;

    // follow the player, but clamp the camera to the maze bounds; near an edge
    // the camera stops and the player keeps moving within the viewport instead
    // of getting stuck against open space
    int newCamX = clampi(playerX - (centerX - 1), 0, mazeW - viewW);
    int newCamY = clampi(playerY - (centerY - 1), 0, mazeH - viewH);
    int sdx = newCamX - camX;
    int sdy = newCamY - camY;
    camX = newCamX;
    camY = newCamY;

    if (sdx != 0 || sdy != 0)
        Scene_scroll(mazeUid, sdx, sdy); // redraws the whole viewport
    else
        Canvas_setChar(oldSX, oldSY, ' '); // erase the player's old cell

    drawPlayer();
    return 0;
}

int main() {
    Canvas_clearScreen();
    Canvas_hideCursor();
    Screen_setEchoEnabled(0);
    Screen_setLineBuffered(0);

    int width = Canvas_getWidth();
    int height = Canvas_getHeight();
    if (width < 20 || height < 12) {
        Canvas_showCursor();
        printf("Terminal too small for the maze demo.\n");
        return 1;
    }

    // reserve the bottom row for the hud
    viewW = width;
    viewH = height - 1;
    centerX = viewW / 2;
    centerY = viewH / 2;

    // the maze is twice the viewport in each direction so there is room to
    // scroll; odd dimensions keep the recursive backtracker grid aligned
    mazeW = viewW * 2 + 1;
    mazeH = viewH * 2 + 1;

    mazeData = Char2DBuilder_createFilled(mazeW, mazeH, '#');
    mazeAnsiData =
        String2DBuilder_createFilledForeground(mazeW, mazeH, 0x202020);

    if (mazeData == 0 || mazeAnsiData == 0) {
        Canvas_showCursor();
        printf("Failed to allocate the maze and/or maze coloring.\n");
        return 1;
    }

    srand((unsigned) time(0));
    generateMaze();

    maze = Scene_createFromData(mazeData, mazeAnsiData);
    if (maze == 0) {
        Canvas_showCursor();
        printf("Failed to create the maze scene.\n");
        return 1;
    }
    mazeUid = Scene_register(maze);

    // start the player on an odd (carved) cell near the middle of the maze
    int startX = oddify(mazeW / 2);
    int startY = oddify(mazeH / 2);
    camX = clampi(startX - (centerX - 1), 0, mazeW - viewW);
    camY = clampi(startY - (centerY - 1), 0, mazeH - viewH);
    playerX = camX + centerX - 1;
    playerY = camY + centerY - 1;
    mazeData[playerY][playerX] = ' '; // guarantee the player is on a passage

    // draw the initial viewport of the maze, then the player on top
    Scene_drawPortion(maze, 1, 1, camX, camY, camX + viewW, camY + viewH);
    drawPlayer();

    Canvas_drawText(2, height, "WASD to move - Q to quit");

    addCmdFXEventListener(CMDFX_EVENT_KEY, onKeyPress);

    while (running) sleepMillis(50);

    // restore the terminal and tear down
    endCmdFXEventLoop();
    sleepMillis(50); // let the event loop drain
    shutdownCmdFXEvents();

    Scene_free(maze); // also frees mazeData

    Canvas_showCursor();
    Screen_setEchoEnabled(1);
    Screen_setLineBuffered(1);

    return 0;
}
