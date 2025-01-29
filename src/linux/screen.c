#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

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