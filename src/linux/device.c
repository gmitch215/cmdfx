#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include <linux/input.h>

#include "cmdfx/device.h"

char* find_linux_event(const char* keyword) {
    FILE *fp = fopen("/proc/bus/input/devices", "r");
    if (!fp) {
        perror("Failed to open /proc/bus/input/devices");
        return 0;
    }

    char line[256];
    int found = 0;
    char eventFile[32];

    while (fgets(line, sizeof(line), fp)) {
        char* upperKeyword = strdup(keyword);
        upperKeyword[0] = toupper(upperKeyword[0]);

        if (strstr(line, keyword) || strstr(line, upperKeyword))
            found = 1;
        
        free(upperKeyword);

        if (found && strstr(line, "Handlers=")) {
            char *eventPos = strstr(line, "event");
            if (eventPos) {
                sscanf(eventPos, "event%d", &eventFile[5]);
                snprintf(eventFile, sizeof(eventFile), "/dev/input/%s", eventPos);
                fclose(fp);

                return strdup(eventFile);
            }
        }
    }

    fclose(fp);
    return 0;
}

int* Device_getKeyboardKeysPressed() {
    char* eventFile = find_linux_event("keyboard");
    if (!eventFile) return 0;

    int fd = open(eventFile, O_RDONLY | O_NONBLOCK);
    if (fd < 0) return 0;

    int* keys = (int*) calloc(256, sizeof(int));
    struct input_event event;

    while (read(fd, &event, sizeof(event)) > 0) {
        if (event.type == EV_KEY) {
            keys[event.code] = event.value;
        }
    }

    close(fd);
    return keys;
}

int* Device_getMouseButtonsPressed() {
    char* eventFile = find_linux_event("mouse");
    if (!eventFile) return 0;

    int fd = open(eventFile, O_RDONLY | O_NONBLOCK);
    if (fd < 0) return 0;

    int* buttons = (int*) calloc(3, sizeof(int));
    struct input_event event;

    while (read(fd, &event, sizeof(event)) > 0) {
        if (event.type == EV_KEY) {
            if (event.code == BTN_LEFT) buttons[0] = event.value;
            if (event.code == BTN_RIGHT) buttons[1] = event.value;
            if (event.code == BTN_MIDDLE) buttons[2] = event.value;
        }
    }

    close(fd);
    return buttons;
}