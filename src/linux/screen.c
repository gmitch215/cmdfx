#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h>

#include "cmdfx/screen.h"

// Screen API

void Screen_getSize(int *width, int *height) {
    int fb = open("/dev/fb0", O_RDONLY);
    if (fb < 0) {
        perror("Failed to open framebuffer device");

        *width = 0;
        *height = 0;
        return;
    }

    struct fb_var_screeninfo screen_info;
    if (ioctl(fb, FBIOGET_VSCREENINFO, &screen_info) < 0) {
        perror("Failed to get screen information");
        close(fb);

        *width = 0;
        *height = 0;
        return;
    }

    *width = screen_info.xres;
    *height = screen_info.yres;

    close(fb);
}

static int fd = -1;
static int cx = 0, cy = 0;

void Screen_getMousePos(int* x, int* y) {
    fd = open("/dev/input/mice", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Failed to open /dev/input/mice");
    }

    if (fd == -1) {
        close(fd);
        return;
    }

    int8_t data[3]; // [button state, dx, dy]
    ssize_t bytes = read(fd, data, sizeof(data));
    if (bytes > 0) {
        cx += data[1];
        cy -= data[2];
    }

    *x = cx;
    *y = cy;
}