#define _GNU_SOURCE

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/input.h>

#include "cmdfx/core/device.h"

char* find_linux_event(const char* keyword) {
    FILE* fp = fopen("/proc/bus/input/devices", "r");
    if (!fp) {
        perror("Failed to open /proc/bus/input/devices");
        return 0;
    }

    char line[256];
    int found = 0;
    char eventFile[32] = {0};

    while (fgets(line, sizeof(line), fp)) {
        if (strcasestr(line, keyword)) found = 1;

        if (found && strstr(line, "Handlers=")) {
            char* eventPos = strstr(line, "event");
            if (eventPos) {
                int eventNum;
                if (sscanf(eventPos, "event%d", &eventNum) == 1) {
                    snprintf(
                        eventFile, sizeof(eventFile), "/dev/input/event%d",
                        eventNum
                    );
                    fclose(fp);
                    return strdup(eventFile);
                }
            }
        }
    }

    fclose(fp);
    return 0;
}

char Device_fromKeyCode(int keyCode) {
    struct input_event ev;
    char* devicePath = find_linux_event("keyboard");
    if (!devicePath) {
        fprintf(stderr, "Keyboard event device not found.\n");
        return '\0';
    }

    // validate device path
    if (strncmp(devicePath, "/dev/input/", 11) != 0) {
        fprintf(stderr, "Invalid device path.\n");
        free(devicePath);
        return '\0';
    }

    int fd = open(devicePath, O_RDONLY);
    free(devicePath);

    if (fd < 0) {
        perror("Failed to open input device");
        return '\0';
    }

    while (read(fd, &ev, sizeof(struct input_event)) > 0) {
        if (ev.type == EV_KEY && ev.value == 1) {
            if (ev.code == keyCode) {
                close(fd);
                return (char) keyCode;
            }
        }
    }

    close(fd);
    return '\0';
}

bool* Device_getKeyboardKeysPressed() {
    char* eventFile = find_linux_event("keyboard");
    if (!eventFile) return 0;

    bool* keys = (bool*) calloc(256, sizeof(bool));
    if (!keys) return 0;

    // validate device path
    if (strncmp(eventFile, "/dev/input/", 11) != 0) {
        fprintf(stderr, "Invalid device path.\n");
        free(eventFile);
        free(keys);
        return 0;
    }

    int fd = open(eventFile, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        fprintf(stderr, "Failed to open input device: %s\n", eventFile);
        perror("Output");
        free(eventFile);
        exit(1);
    }

    struct input_event event;

    int state = read(fd, &event, sizeof(event));
    if (state < 0) {
        if (errno == EAGAIN) {
            close(fd);
            free(eventFile);
            return keys;
        }

        perror("Failed to read input event");
        free(eventFile);
        exit(1);
    }

    while (state > 0) {
        if (event.type == EV_KEY) {
            keys[event.code] = (bool) event.value;
        }

        state = read(fd, &event, sizeof(event));
    }

    close(fd);
    free(eventFile);
    return keys;
}

bool* Device_getMouseButtonsPressed() {
    char* eventFile = find_linux_event("mouse");
    if (!eventFile) return 0;

    bool* buttons = (bool*) calloc(3, sizeof(bool));
    if (!buttons) {
        free(eventFile);
        return 0;
    }

    // validate device path
    if (strncmp(eventFile, "/dev/input/", 11) != 0) {
        fprintf(stderr, "Invalid device path.\n");
        free(eventFile);
        free(buttons);
        return 0;
    }

    int fd = open(eventFile, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        fprintf(stderr, "Failed to open input device: %s\n", eventFile);
        perror("Output");
        free(eventFile);
        exit(1);
    }

    struct input_event event;

    while (read(fd, &event, sizeof(event)) > 0) {
        if (event.type == EV_KEY) {
            if (event.code == BTN_LEFT) buttons[0] = event.value;
            if (event.code == BTN_RIGHT) buttons[1] = event.value;
            if (event.code == BTN_MIDDLE) buttons[2] = event.value;
        }
    }

    close(fd);
    free(eventFile);
    return buttons;
}
