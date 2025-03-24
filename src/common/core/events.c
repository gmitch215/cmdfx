#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/events.h"

#define MAX_LISTENERS 1024

EventCallback*** _listeners = 0;
unsigned int* _listenerSizes = 0;

void initCmdFXEvents() {
    if (_listeners != 0) return;

    _listeners = calloc(MAX_LISTENERS, sizeof(EventCallback**));
    _listenerSizes = calloc(MAX_LISTENERS, sizeof(unsigned int));

    if (!_listeners || !_listenerSizes) {
        fprintf(stderr, "Failed to initialize CmdFX events.\n");
        exit(1);
    }

    beginCmdFXEventLoop();
}

void shutdownCmdFXEvents() {
    if (_listeners == 0) return;

    for (int i = 0; i < MAX_LISTENERS; i++) {
        EventCallback** list = _listeners[i];
        if (list == 0) continue;

        free(list);
    }

    free(_listeners);
    _listeners = 0;
}

int addCmdFXEventListener(unsigned int id, EventCallback callback) {
    if (id >= MAX_LISTENERS) return -1;

    initCmdFXEvents();
    EventCallback** list = _listeners[id];
    unsigned int size = _listenerSizes[id];

    EventCallback* ptr = malloc(sizeof(EventCallback));
    if (!ptr) return -1;
    *ptr = callback;

    if (list == 0) {
        list = malloc(sizeof(EventCallback*));
        if (!list) {
            free(ptr);
            return -1;
        }

        _listeners[id] = list;
        _listenerSizes[id] = 1;
        list[0] = ptr;
        return 0;
    }

    for (unsigned int i = 0; i < size; i++)
        if (list[i] == 0) {
            list[i] = ptr;
            return i;
        }

    unsigned int newSize = size + 1;
    EventCallback** newList = realloc(list, sizeof(EventCallback*) * newSize);
    if (!newList) return -1;

    _listeners[id] = newList;
    _listenerSizes[id] = newSize;
    newList[size] = ptr;
    return size;
}

EventCallback* getCmdFXEventListener(unsigned int eventId, unsigned int listenerId) {
    if (eventId >= MAX_LISTENERS) return 0;
    if (_listeners == 0) return 0;

    return _listeners[eventId][listenerId];
}

int removeCmdFXEventListener(unsigned int eventId, unsigned int listenerId) {
    if (eventId >= MAX_LISTENERS) return 0;
    if (_listeners == 0) return 1;

    EventCallback** list = _listeners[eventId];
    unsigned int size = _listenerSizes[eventId];
    if (list == 0 || listenerId >= size || list[listenerId] == 0) return 0;

    list[listenerId] = 0;

    unsigned int nonNullCount = 0;
    for (unsigned int i = 0; i < size; i++) {
        if (list[i] != NULL) {
            list[nonNullCount++] = list[i];
        }
    }

    if (nonNullCount == size) return 1;

    if (nonNullCount == 0) {
        free(list);
        _listeners[eventId] = NULL;
        _listenerSizes[eventId] = 0;
    } else {
        EventCallback** newList = realloc(list, sizeof(EventCallback*) * nonNullCount);
        if (newList || nonNullCount == 0) {
            _listeners[eventId] = newList;
            _listenerSizes[eventId] = nonNullCount;
        }
    }

    return 1;
}

const EventCallback** dispatchCmdFXEvent(CmdFX_Event* event) {
    if (!event || event->id >= MAX_LISTENERS) return 0;

    EventCallback** list = _listeners[event->id];
    unsigned int size = _listenerSizes[event->id];

    if (!list) return 0;

    const EventCallback** called = malloc(sizeof(EventCallback*) * (size + 1));
    if (!called) return 0;

    unsigned int count = 0;

    for (unsigned int i = 0; i < size; i++) {
        if (list[i] != 0) {
            EventCallback callback = *list[i];
            int result = callback(event);
            if (result == 0)
                called[count++] = list[i];
        }
    }

    if (count == 0) {
        free(called);
        return 0;
    }

    called[count] = 0;
    return called;
}