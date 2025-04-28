#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/ui/path.h"

// from is inclusive, to is exclusive
static char* _substring(const char* str, int from, int to) {
    if (str == 0 || from > to || to > strlen(str)) return 0;

    int len = to - from;
    char* result = malloc(len + 1);
    if (result == 0) return 0;

    strncpy(result, str + from, len);
    result[len] = '\0';

    return result;
}

int Canvas_path(const char* path, char ch) {
    if (path == 0) return -1;

    int l = strlen(path);
    int x = Canvas_getCursorX();
    int y = Canvas_getCursorY();

    for (int i = 0; i < l; i++) {
        char c = path[i];
        if (c == ' ' || c == ',') continue;

        Canvas_setCursor(x, y); // reset cursor to current position
        switch (c) {
            #pragma region Path Commands
            case CMDFX_PATH_COMMAND_MOVETO_ABSOLUTE: {
                i += 2; // move to x coordinate from M
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                x = atoi(xs);
                free(xs);

                i++; // move to y coordinate from space after x
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                y = atoi(ys);
                free(ys);

                Canvas_setCursor(x, y);
                break;
            }
            case CMDFX_PATH_COMMAND_MOVETO_RELATIVE: {
                i += 2; // move to dx coordinate from m
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                x += atoi(xs);
                free(xs);

                i++; // move to dy coordinate from space after dx
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                y += atoi(ys);
                free(ys);

                Canvas_setCursor(x, y);
                break;
            }
            case CMDFX_PATH_COMMAND_LINETO_ABSOLUTE: {
                i += 2; // move to x coordinate from L
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int x2 = atoi(xs);
                free(xs);

                i++; // move to y coordinate from space after x
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int y2 = atoi(ys);
                free(ys);

                Canvas_line(x, y, x2, y2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_LINETO_RELATIVE: {
                i += 2; // move to dx coordinate from l
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int dx = atoi(xs);
                free(xs);

                i++; // move to dy coordinate from space after dx
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int dy = atoi(ys);
                free(ys);

                Canvas_line(x, y, x + dx, y + dy, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_HLINETO_ABSOLUTE: {
                i += 2; // move to x coordinate from H
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int x2 = atoi(xs);
                free(xs);

                Canvas_hLine(x, y, x2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_HLINETO_RELATIVE: {
                i += 2; // move to dx coordinate from h
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int dx = atoi(xs);
                free(xs);

                Canvas_hLine(x, y, x + dx, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_VLINETO_ABSOLUTE: {
                i += 2; // move to y coordinate from V
                int fromY = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* ys = _substring(path, fromY, i);
                int y2 = atoi(ys);
                free(ys);

                Canvas_vLine(x, y, y2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_VLINETO_RELATIVE: {
                i += 2; // move to dy coordinate from v
                int fromY = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* ys = _substring(path, fromY, i);
                int dy = atoi(ys);
                free(ys);

                Canvas_vLine(x, y, y + dy, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_RECT_ABSOLUTE: {
                i += 2; // move to x1 coordinate from R
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int x1 = atoi(xs);
                free(xs);

                i++; // move to y1 coordinate from space after x1
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int y1 = atoi(ys);
                free(ys);

                i++; // move to x2 coordinate from space after y1
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int x2 = atoi(xs2);
                free(xs2);

                i++; // move to y2 coordinate from space after x2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int y2 = atoi(ys2);
                free(ys2);

                Canvas_rect(x1, y1, x2, y2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_ELLIPSE_ABSOLUTE: {
                i += 2; // move to cx coordinate from E
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int cx = atoi(xs);
                free(xs);

                i++; // move to cy coordinate from space after cx
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int cy = atoi(ys);
                free(ys);

                i++; // move to rx coordinate from space after cy
                int fromRx = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromRx, i);
                int rx = atoi(xs2);
                free(xs2);

                i++; // move to ry coordinate from space after rx
                int fromRy = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromRy, i);
                int ry = atoi(ys2);
                free(ys2);

                Canvas_ellipse(cx, cy, rx, ry, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_ELLIPSE_RELATIVE: {
                i += 2; // move to dx coordinate from e
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int dx = atoi(xs);
                free(xs);

                i++; // move to dy coordinate from space after dx
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int dy = atoi(ys);
                free(ys);

                i++; // move to rx coordinate from space after dy
                int fromRx = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromRx, i);
                int rx = atoi(xs2);
                free(xs2);

                i++; // move to ry coordinate from space after rx
                int fromRy = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromRy, i);
                int ry = atoi(ys2);
                free(ys2);

                Canvas_ellipse(x + dx, y + dy, rx, ry, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_RECT_RELATIVE: {
                i += 2; // move to dx1 coordinate from r
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int dx1 = atoi(xs);
                free(xs);

                i++; // move to dy1 coordinate from space after dx1
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int dy1 = atoi(ys);
                free(ys);

                i++; // move to dx2 coordinate from space after dy1
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int dx2 = atoi(xs2);
                free(xs2);

                i++; // move to dy2 coordinate from space after dx2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int dy2 = atoi(ys2);
                free(ys2);

                Canvas_rect(x + dx1, y + dy1, x + dx2, y + dy2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_QUAD_ABSOLUTE: {
                i += 2; // move to x1 coordinate from Q
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int x1 = atoi(xs);
                free(xs);

                i++; // move to y1 coordinate from space after x1
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int y1 = atoi(ys);
                free(ys);

                i++; // move to x2 coordinate from space after y1
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int x2 = atoi(xs2);
                free(xs2);

                i++; // move to y2 coordinate from space after x2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int y2 = atoi(ys2);
                free(ys2);

                Canvas_quad(x, y, x1, y1, x2, y2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_QUAD_RELATIVE: {
                i += 2; // move to dx1 coordinate from q
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int dx1 = atoi(xs);
                free(xs);

                i++; // move to dy1 coordinate from space after dx1
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int dy1 = atoi(ys);
                free(ys);

                i++; // move to dx2 coordinate from space after dy1
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int dx2 = atoi(xs2);
                free(xs2);

                i++; // move to dy2 coordinate from space after dx2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int dy2 = atoi(ys2);
                free(ys2);

                Canvas_quad(x, y, x + dx1, y + dy1, x + dx2, y + dy2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_CUBIC_ABSOLUTE: {
                i += 2; // move to x1 coordinate from C
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int x1 = atoi(xs);
                free(xs);

                i++; // move to y1 coordinate from space after x1
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int y1 = atoi(ys);
                free(ys);

                i++; // move to x2 coordinate from space after y1
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int x2 = atoi(xs2);
                free(xs2);

                i++; // move to y2 coordinate from space after x2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int y2 = atoi(ys2);
                free(ys2);

                i++; // move to x3 coordinate from space after y2
                int fromX3 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs3 = _substring(path, fromX3, i);
                int x3 = atoi(xs3);
                free(xs3);

                i++; // move to y3 coordinate from space after x3
                int fromY3 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys3 = _substring(path, fromY3, i);
                int y3 = atoi(ys3);
                free(ys3);

                Canvas_cubic(x, y, x1, y1, x2, y2, x3, y3, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_CUBIC_RELATIVE: {
                i += 2; // move to dx1 coordinate from c
                int fromX = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromX, i);
                int dx1 = atoi(xs);
                free(xs);

                i++; // move to dy1 coordinate from space after dx1
                int fromY = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromY, i);
                int dy1 = atoi(ys);
                free(ys);

                i++; // move to dx2 coordinate from space after dy1
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int dx2 = atoi(xs2);
                free(xs2);

                i++; // move to dy2 coordinate from space after dx2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int dy2 = atoi(ys2);
                free(ys2);

                i++; // move to dx3 coordinate from space after dy2
                int fromX3 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs3 = _substring(path, fromX3, i);
                int dx3 = atoi(xs3);
                free(xs3);

                i++; // move to dy3 coordinate from space after dx3
                int fromY3 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys3 = _substring(path, fromY3, i);
                int dy3 = atoi(ys3);
                free(ys3);

                Canvas_cubic(x, y, x + dx1, y + dy1, x + dx2, y + dy2, x + dx3, y + dy3, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_ARC_ABSOLUTE: {
                i += 2; // move to rx coordinate from A
                int fromRx = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromRx, i);
                int rx = atoi(xs);
                free(xs);

                i++; // move to ry coordinate from space after rx
                int fromRy = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromRy, i);
                int ry = atoi(ys);
                free(ys);

                i++; // move to xrot coordinate from space after ry
                int fromXAR = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xsAR = _substring(path, fromXAR, i);
                double xrot = atof(xsAR);
                free(xsAR);

                i++; // move to arcflag coordinate from space after xrot
                int arcflag = path[i];

                i++; // move to sweepflag coordinate from space after arcflag
                int sweepflag = path[i];

                i++; // move to x2 coordinate from space after sweepflag
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int x2 = atoi(xs2);
                free(xs2);

                i++; // move to y2 coordinate from space after x2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int y2 = atoi(ys2);
                free(ys2);

                Canvas_arc(x, y, rx, ry, xrot, arcflag, sweepflag, x2, y2, ch);
                break;
            }
            case CMDFX_PATH_COMMAND_ARC_RELATIVE: {
                i += 2; // move to rx coordinate from a
                int fromRx = path[i];
                while (path[i] != ' ' || path[i] != ',') i++;
                char* xs = _substring(path, fromRx, i);
                int rx = atoi(xs);
                free(xs);

                i++; // move to ry coordinate from space after rx
                int fromRy = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys = _substring(path, fromRy, i);
                int ry = atoi(ys);
                free(ys);

                i++; // move to xrot coordinate from space after ry
                int fromXAR = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xsAR = _substring(path, fromXAR, i);
                double xrot = atof(xsAR);
                free(xsAR);

                i++; // move to arcflag coordinate from space after xrot
                int arcflag = path[i];

                i++; // move to sweepflag coordinate from space after arcflag
                int sweepflag = path[i];

                i++; // move to dx2 coordinate from space after sweepflag
                int fromX2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* xs2 = _substring(path, fromX2, i);
                int dx2 = atoi(xs2);
                free(xs2);

                i++; // move to dy2 coordinate from space after dx2
                int fromY2 = path[i];
                while (path[i] == ' ' || path[i] == ',') i++;
                char* ys2 = _substring(path, fromY2, i);
                int dy2 = atoi(ys2);
                free(ys2);

                Canvas_arc(x, y, rx, ry, xrot, arcflag, sweepflag, x + dx2, y + dy2, ch);
                break;
            }
            default: {
                break;
            }
            #pragma endregion
        }
    }

    return 0;
}