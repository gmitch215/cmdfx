#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

#include "cmdfx/device.h"

#define KEYBOARD_EVENT "/dev/input/event3"
#define MOUSE_EVENT "/dev/input/event4"

int Device_getKeyboardKeyPressed() {
    int fd = open(KEYBOARD_EVENT, O_RDONLY | O_NONBLOCK);
    if (fd < 0) return -1;

    struct input_event event;
    while (read(fd, &event, sizeof(event)) > 0) {
        if (event.type == EV_KEY && event.value == 1) {
            close(fd);
            return event.code;
        }
    }

    close(fd);
    return -1;
}

int Device_getMouseButtonPressed() {
    int fd = open(MOUSE_EVENT, O_RDONLY | O_NONBLOCK);
    if (fd < 0) return -1;

    struct input_event event;
    while (read(fd, &event, sizeof(event)) > 0) {
        if (event.type == EV_KEY && event.value == 1) {
            close(fd);
            return event.code;
        }
    }

    close(fd);
    return -1;
}