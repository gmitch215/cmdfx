/**
 * @file events.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Events API for CmdFX.
 * @version 0.1.0
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The time between event loop iterations, in milliseconds.
 * 
 * This value is used to determine how often the event loop should check for
 * events. A lower value will result in more frequent event checks, but may
 * consume more CPU resources. A higher value will result in less frequent event
 * checks, but may cause events to be missed. This is usually set to avoid a
 * "busy loop" that consumes too many resources.
 * 
 * The value used for CmdFX events is 10 milliseconds.
 */
#define EVENT_TICK 10

/**
 * Called when the terminal is resized.
 * The data is a pointer to a `struct CmdFX_ResizeEvent`.
 */
#define CMDFX_EVENT_RESIZE 0

/**
 * The payload for the `CMDFX_EVENT_RESIZE` event.
 */
typedef struct CmdFX_ResizeEvent {
    /**
     * The previous width of the terminal.
     */
    int prevWidth;
    /**
     * The previous height of the terminal.
     */
    int prevHeight;
    /**
     * The new width of the terminal.
     */
    int newWidth;
    /**
     * The new height of the terminal.
     */
    int newHeight;
} CmdFX_ResizeEvent;

struct CmdFX_Event;

/**
 * Represents an event callback.
 */
typedef int (*EventCallback)(struct CmdFX_Event* event);

/**
 * The main event structure for CmdFX.
 * @param id The event ID.
 * @param time The time the event was created, in milliseconds.
 * @param data A pointer to the data associated with the event.
 */
typedef struct CmdFX_Event {
    unsigned int id;
    unsigned long long time;
    void* data;
} CmdFX_Event;

/**
 * Adds an event listener.
 * @param id The event ID.
 * @param callback The event callback.
 * @return A unique ID for the event listener, or -1 if an error occurred.
 */
int addCmdFXEventListener(unsigned int id, EventCallback callback);

/**
 * Gets an event listener.
 * @param eventId The ID of the event to remove the listener from.
 * @param listenerId The ID of the event listener to retrieve.
 * @return The event listener, or NULL if the listener does not exist.
 */
EventCallback* getCmdFXEventListener(unsigned int eventId, unsigned int listenerId);

/**
 * Removes an event listener.
 * @param eventId The ID of the event to remove the listener from.
 * @param listenerId The ID of the event listener to remove.
 * @return 1 if the listener was removed successfully, or 0 if an error occurred.
 */
int removeCmdFXEventListener(unsigned int eventId, unsigned int listenerId);

/**
 * Dispatches an event.
 * @param event The event to dispatch.
 * @return Addresses of the event listeners that were called.
 */
const EventCallback** dispatchCmdFXEvent(CmdFX_Event* event);

/**
 * Starts the event loop for CmdFX. This method is called automatically if a listener is added.
 * @return 1 if the event loop was started successfully, 0 if an error occurred or the event loop is already running.
 */
int beginCmdFXEventLoop();

/**
 * Ends the event loop for CmdFX.
 * @return 1 if the event loop was ended successfully, 0 if an error occurred or the event loop is not running.
 */
int endCmdFXEventLoop();

#ifdef __cplusplus
}
#endif