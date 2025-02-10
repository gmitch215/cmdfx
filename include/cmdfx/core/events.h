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

#pragma region Events

/**
 * @brief Called when the terminal is resized.
 * 
 * The data is a pointer to a `struct CmdFX_ResizeEvent`.
 */
#define CMDFX_EVENT_RESIZE 0

/**
 * @brief The payload for the `CMDFX_EVENT_RESIZE` event.
 */
typedef struct CmdFX_ResizeEvent {
    /**
     * @brief The previous width of the terminal.
     */
    int prevWidth;
    /**
     * @brief The previous height of the terminal.
     */
    int prevHeight;
    /**
     * @brief The new width of the terminal.
     */
    int newWidth;
    /**
     * @brief The new height of the terminal.
     */
    int newHeight;
} CmdFX_ResizeEvent;

/**
 * @brief Called when a key is pressed or released.
 * 
 * The data is a pointer to a `struct CmdFX_KeyEvent`.
 */
#define CMDFX_EVENT_KEY 1

/**
 * @brief The payload for the `CMDFX_EVENT_KEY` event.
 */
typedef struct CmdFX_KeyEvent {
    /**
     * @brief The key code, or 0 if no key is pressed.
     */
    int keyCode;
    /**
     * @brief The character representation of the key.
     * 
     * This value is 0 if the key does not have a character representation.
     */
    char keyChar;
    /**
     * @brief The state of the key.
     * 
     * The state is 1 if the key is pressed, or 0 if the key is released.
     */
    int state;
} CmdFX_KeyEvent;

/**
 * @brief Called when the mouse is moved or clicked.
 * 
 * The data is a pointer to a `struct CmdFX_MouseEvent`.
 */
#define CMDFX_EVENT_MOUSE 2

/**
 * @brief The payload for the `CMDFX_EVENT_MOUSE` event.
 */
typedef struct CmdFX_MouseEvent {
    /**
     * @brief The button that was pressed, or -1 if no button was pressed.
     * 
     * On most systems, the button values are as follows:
     * - 0: Left mouse button
     * - 1: Middle mouse button
     * - 2: Right mouse button
     */
    int button;
    /**
     * @brief Whether the button was pressed or released.
     */
    int state;
    /**
     * @brief The previous x position of the mouse.
     */
    int prevX;
    /**
     * @brief The x position of the mouse.
     */
    int x;
    /**
     * @brief The previous y position of the mouse.
     */
    int prevY;
    /**
     * @brief The y position of the mouse.
     */
    int y;
} CmdFX_MouseEvent;

#pragma endregion

struct CmdFX_Event;

/**
 * @brief Represents an event callback.
 */
typedef int (*EventCallback)(struct CmdFX_Event* event);

/**
 * @brief The main event structure for CmdFX.
 * @param id The event ID.
 * @param time The time the event was created, in milliseconds.
 * @param data A pointer to the data associated with the event.
 */
typedef struct CmdFX_Event {
    /**
     * @brief The event ID.
     */
    unsigned int id;
    /**
     * @brief The time the event was created, in milliseconds.
     */
    unsigned long long time;
    /**
     * @brief A pointer to the data associated with the event.
     */
    void* data;
} CmdFX_Event;

/**
 * @brief Adds an event listener.
 * 
 * The listener will be called when the event with the specified ID is dispatched.
 * The library has an internal limit of 1,024 listeners across all events.
 * @param id The event ID.
 * @param callback The event callback.
 * @return A unique ID for the event listener, or -1 if an error occurred.
 */
int addCmdFXEventListener(unsigned int id, EventCallback callback);

/**
 * @brief Gets an event listener.
 * @param eventId The ID of the event to remove the listener from.
 * @param listenerId The ID of the event listener to retrieve.
 * @return The event listener, or NULL if the listener does not exist.
 */
EventCallback* getCmdFXEventListener(unsigned int eventId, unsigned int listenerId);

/**
 * @brief Removes an event listener.
 * @param eventId The ID of the event to remove the listener from.
 * @param listenerId The ID of the event listener to remove.
 * @return 1 if the listener was removed successfully, or 0 if an error occurred.
 */
int removeCmdFXEventListener(unsigned int eventId, unsigned int listenerId);

/**
 * @brief Dispatches an event.
 * @param event The event to dispatch.
 * @return Addresses of the event listeners that were called.
 */
const EventCallback** dispatchCmdFXEvent(CmdFX_Event* event);

/**
 * @brief Starts the event loop for CmdFX. 
 * 
 * This method is called automatically if a listener is added.
 * @return 1 if the event loop was started successfully, 0 if an error occurred or the event loop is already running.
 */
int beginCmdFXEventLoop();

/**
 * @brief Ends the event loop for CmdFX.
 * @return 1 if the event loop was ended successfully, 0 if an error occurred or the event loop is not running.
 */
int endCmdFXEventLoop();

#ifdef __cplusplus
}
#endif