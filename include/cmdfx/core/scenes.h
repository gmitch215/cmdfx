/**
 * @file scenes.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Scenes API for CmdFX
 * @version 0.1.3
 * @date 2025-03-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents the maximum number of registered scenes that can be stored.
 *
 * This is the maximum number of scenes that can be registered with the Scene
 * Engine at any given time. Registered scenes are stored in a static array,
 * which is used to keep track of additional data scenes may have, such as
 * buttons.
 */
#define MAX_REGISTERED_SCENES 1024

/**
 * @brief Represents a CmdFX Scene.
 *
 * A CmdFX Scene is a storage buffer for different characters on the screen.
 * It is drawn using the cnvas API and should be used when displaying large
 * amounts of characters at a time.
 */
typedef struct CmdFX_Scene {
    /**
     * @brief The unique identifier for the scene.
     *
     * The unique identifier is a number that is assigned to the scene when
     * it is created. This number is used to identify the scene when it is
     * registered to the Scene Engine.
     *
     * If this number is `-1`, the scene has not been registered to the
     * Scene Engine.
     */
    int uid;
    /**
     * @brief Represents the width of the scene.
     *
     * The width of the scene is the number of characters that can be
     * displayed horizontally. This is the same as the width of each array
     * in the data field.
     */
    int width;
    /**
     * @brief Represents the height of the scene.
     *
     * The height of the scene is the number of characters that can be
     * displayed vertically. This is the same as the number of arrays in the
     * data field.
     */
    int height;
    /**
     * @brief Represents the data of the scene.
     *
     * The data field is a 2D array of characters that represent the scene.
     * Each array in the data field represents a row of characters on the
     * screen. The width of the scene is the same as the width of each array
     * in the data field. The height of the scene is the same as the number
     * of arrays in the data field.
     *
     * These characters are drawn using the Canvas API and will therefore
     * disappear if a Sprite is drawn over them. The Scene Engine will
     * automatically redraw the scene each frame if necessary.
     */
    char** data;
    /**
     * @brief Represents the ANSI data of the scene.
     *
     * The ANSI data field is a 2D array of ANSI characters that represent
     * the scene. Each array in the ANSI data field represents a row of
     * characters on the screen. The width of the scene is the same as the
     * width of each array in the ANSI data field. The height of the scene
     * is the same as the number of arrays in the ANSI data field.
     *
     * These characters are drawn using the Canvas API and will therefore
     * disappear if a Sprite is drawn over them. The Scene Engine will
     * automatically redraw the scene each frame if necessary.
     */
    char*** ansiData;
    /**
     * @brief Gets the x-coordinate of the scene.
     *
     * The x-coordinate of the scene is the position of the top-left corner
     * of the scene on the screen. This is the same as the x-coordinate of
     * the scene when it is drawn using the `Scene_draw` function. If the
     * scene is not currently drawn, it will be
     * `-1`.
     */
    int x;
    /**
     * @brief Gets the y-coordinate of the scene.
     *
     * The y-coordinate of the scene is the position of the top-left corner
     * of the scene on the screen. This is the same as the y-coordinate of
     * the scene when it is drawn using the `Scene_draw` function. If the
     * scene is not currently drawn, it will be
     * `-1`.
     */
    int y;
    /**
     * @brief Represents the Z index of the scene.
     *
     * The Z index of the scene is the order in which the scene is drawn on
     * the screen. Scenes with a higher Z index will be drawn on top of
     * scenes with a lower Z index.
     */
    int z;
} CmdFX_Scene;

/**
 * @brief Gets a list of all scenes that are drawn on the screen.
 * @return An array of pointers to all scenes that are drawn on the screen.
 */
CmdFX_Scene** Canvas_getDrawnScenes();

/**
 * @brief Gets the number of scenes that are drawn on the screen.
 * @return The number of scenes that are drawn on the screen.
 */
int Canvas_getDrawnScenesCount();

/**
 * @brief Creates an empty scene with a specified width and height.
 *
 * This function creates a new scene with the specified width and height.
 * The scene will be empty, meaning that all characters in the data field
 * will be set to `' '`.
 *
 * The width and height parameters must be greater than 0. If either
 * parameter is less than or equal to 0, the function will return `NULL`.
 *
 * @param width The width of the scene.
 * @param height The height of the scene.
 * @return A pointer to the newly created scene, or `NULL` if the scene
 * could not be created.
 */
CmdFX_Scene* Scene_create(int width, int height);

/**
 * @brief Creates a filled scene with a specified width, height, and
 * character.
 *
 * This function creates a new scene with the specified width and height.
 * The scene will be filled with the specified character. The ANSI data will
 * be set to the specified ANSI string.
 *
 * The width and height parameters must be greater than 0. If either
 * parameter is less than or equal to 0, the function will return `NULL`.
 *
 * @param width The width of the scene.
 * @param height The height of the scene.
 * @param c The character to fill the scene with.
 * @param ansi The ANSI string to fill the scene with.
 * @param z The Z index of the scene.
 * @return A pointer to the newly created scene, or `NULL` if the scene
 * could not be created.
 */
CmdFX_Scene* Scene_createFilled(
    int width, int height, char c, char* ansi, int z
);

/**
 * @brief Creates a scene from a 2D array of characters and ANSI strings.
 *
 * This function creates a new scene from the specified 2D array of
 * characters and ANSI strings. The width and height of the scene will be
 * set to the width and height of the 2D array. Therefore, the arrays must
 * be properly null-teminated with a `NULL` character at the end of each
 * array.
 *
 * The bounds will be based on the `data` array. If the bounds do not match,
 * the function will return `NULL`.
 *
 * The data parameter must not be `NULL`. If data is `NULL`, the function
 * will return `NULL`. `ansiData` can optionally be `NULL` to represent no
 * color.
 *
 * @param data The 2D array of characters that represent the scene.
 * @param ansiData The 2D array of ANSI strings that represent the scene.
 * @return A pointer to the newly created scene, or `NULL` if the scene
 * could not be created.
 */
CmdFX_Scene* Scene_createFromData(char** data, char*** ansiData);

/**
 * @brief Clears a scene of its data.
 *
 * This function clears the specified scene of its data. This means that all
 * characters in the data field will be set to 0, making `Scene_isEmpty`
 * return `1`.
 *
 * @param scene The scene to clear.
 * @return `0` if the scene was cleared successfully, or `-1` if an error
 * occurred.
 */
int Scene_clear(CmdFX_Scene* scene);

/**
 * @brief Draws a scene at the specified position on the screen.
 *
 * This function draws the specified scene at the specified position on the
 * screen. The scene will be drawn using the Canvas API, which means that it
 * will disappear if a Sprite is drawn over it. The Scene Engine will
 * automatically redraw the scene each frame if necessary.
 *
 * @param scene The scene to draw.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was drawn successfully, or `-1` if an error
 * occurred.
 */
int Scene_draw(CmdFX_Scene* scene, int x, int y);

/**
 * @brief Draws a portion of a scene at the specified position on the
 * screen.
 *
 * This function draws a portion of the specified scene at the specified
 * position on the screen. The portion is defined by the specified x, y,
 * width, and height parameters. The scene will be drawn using the Canvas
 * API, which means that it will disappear if a Sprite is drawn over it. The
 * Scene Engine will automatically redraw the scene each frame if necessary.
 *
 * The x and y parameters must be greater than or equal to 0. If either
 * parameter is less than 0, the function will return `-1`.
 *
 * @param scene The scene to draw.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @param sx The x-coordinate of the top-left corner of the portion to draw.
 * @param sy The y-coordinate of the top-left corner of the portion to draw.
 * @param width The width of the portion to draw.
 * @param height The height of the portion to draw.
 * @return `0` if the scene was drawn successfully, or `-1` if an error
 * occurred.
 */
int Scene_drawPortion(
    CmdFX_Scene* scene, int x, int y, int sx, int sy, int width, int height
);

/**
 * @brief Gets the scene at the specified position on the screen.
 *
 * This function gets the top-most scene at the specified position on the
 * screen. The scene will be drawn using the Canvas API, which means that it
 * will disappear if a Sprite is drawn over it. The Scene Engine will
 * automatically redraw the scene each frame if necessary.
 *
 * The x and y parameters must be greater than or equal to 0. If either
 * parameter is less than 0, the function will return `NULL`.
 *
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return A pointer to the scene at the specified position, or `NULL` if no
 * scene was found.
 */
CmdFX_Scene* Scene_getSceneAt(int x, int y);

/**
 * @brief Checks if the specified scene is on top of the screen.
 *
 * This function checks if the specified scene is on top of the screen. A
 * scene is considered to be on top if it is the top-most scene at its
 * position on the screen according to `scene->z`.
 *
 * @param scene The scene to check.
 * @return `1` if the scene is on top, or `0` if the scene is not on top.
 */
int Scene_isOnTop(CmdFX_Scene* scene);

/**
 * @brief Checks if the specified scene is on top at the specified position
 * on the screen.
 *
 * This function checks if the specified scene is on top at the specified
 * position on the screen. A scene is considered to be on top if it is the
 * top-most scene at its position on the screen according to `scene->z`.
 *
 * @param scene The scene to check.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `1` if the scene is on top, or `0` if the scene is not on top.
 */
int Scene_isOnTopAt(CmdFX_Scene* scene, int x, int y);

/**
 * @brief Checks if the specified scene is on the bottom of the screen.
 *
 * This function checks if the specified scene is on the bottom of the
 * screen. A scene is considered to be on the bottom if it is the
 * bottom-most scene at its position on the screen according to `scene->z`.
 *
 * @param scene The scene to check.
 * @return `1` if the scene is on the bottom, or `0` if the scene is not on
 * bottom.
 */
int Scene_isOnBottom(CmdFX_Scene* scene);

/**
 * @brief Checks if the specified scene is on the bottom at the specified
 * position on the screen.
 *
 * This function checks if the specified scene is on the bottom at the
 * specified position on the screen. A scene is considered to be on the
 * bottom if it is the bottom-most scene at its position on the screen
 * according to `scene->z`.
 *
 * @param scene The scene to check.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `1` if the scene is on bottom, or `0` if the scene is not on
 * bottom.
 */
int Scene_isOnBottomAt(CmdFX_Scene* scene, int x, int y);

/**
 * @brief Removes a scene from the screen.
 *
 * This function removes the specified scene from the screen. The scene will
 * no longer be drawn on the screen, and the Scene Engine will stop
 * automatically redrawing it each frame.
 *
 * @param scene The scene to remove.
 * @return `0` if the scene was removed successfully, or `-1` if an error
 * occurred.
 */
int Scene_remove(CmdFX_Scene* scene);

/**
 * @brief Checks if the specified scene is empty.
 *
 * This function checks if the specified scene is empty. A scene is
 * considered empty if all characters in the data field are set to 0.
 *
 * @param scene The scene to check.
 * @return `1` if the scene is empty, or `0` if the scene is not empty.
 */
int Scene_isEmpty(CmdFX_Scene* scene);

/**
 * @brief Removes all of the currently drawn scenes, only leaving this
 * scene.
 *
 * This function removes all of the currently drawn scenes from the screen,
 * leaving only the specified scene.
 *
 * @param scene The scene to leave on the screen.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was switched to successfully, or `-1` if an
 * error occurred.
 */
int Scene_switchTo(CmdFX_Scene* scene, int x, int y);

/**
 * @brief Destroys the scene, freeing the memory allocated.
 *
 * This function frees the memory allocated for the specified scene. After
 * calling this function, the scene should no longer be used.
 *
 * @param scene The scene to free.
 * @return `0` if the scene was freed successfully, or `-1` if an error
 * occurred.
 */
int Scene_free(CmdFX_Scene* scene);

/**
 * @brief Gets a list of all registered scenes.
 *
 * @return An array of pointers to all registered scenes.
 */
CmdFX_Scene** Scene_getRegisteredScenes();

/**
 * @brief Gets the number of registered scenes.
 * @return The number of registered scenes.
 */
int Scene_getRegisteredScenesCount();

/**
 * @brief Gets a registered scene by its unique identifier.
 *
 * This function gets the registered scene with the specified unique
 * identifier. The unique identifier is the same as the `uid` field of the
 * scene. If the scene is not registered, the function will return `NULL`.
 *
 * @param uid The unique identifier of the scene to get.
 * @return A pointer to the registered scene, or `NULL` if the scene was not
 * found.
 */
CmdFX_Scene* Scene_getRegisteredScene(int uid);

/**
 * @brief Registers a scene with the Scene Engine.
 *
 * This function registers the specified scene with the Scene Engine. This
 * allows the Scene Engine to keep track of the scene and automatically
 * redraw it each frame if necessary.
 *
 * @param scene The scene to register.
 * @return `scene->uid`, or `-1` if an error occurred.
 */
int Scene_register(CmdFX_Scene* scene);

/**
 * @brief Unregisters a scene with the Scene Engine.
 *
 * This function unregisters the specified scene with the Scene Engine. This
 * allows the Scene Engine to stop keeping track of the scene and stop
 * automatically redrawing it each frame.
 *
 * @param scene The scene to unregister.
 * @return the old `scene->uid`, or `-1` if an error occurred.
 */
int Scene_unregister(CmdFX_Scene* scene);

/**
 * @brief Draws a registered scene at the specified position on the screen.
 *
 * This function draws the registered scene with the specified unique
 * identifier at the specified position on the screen. The scene will be
 * drawn using the Canvas API, which means that it will disappear if a
 * Sprite is drawn over it. The Scene Engine will automatically redraw the
 * scene each frame if necessary.
 *
 * @param uid The unique identifier of the scene to draw.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was drawn successfully, or `-1` if an error
 * occurred.
 */
int Scene_drawRegistered(int uid, int x, int y);

/**
 * @brief Removes all of the currently drawn scenes, only leaving the
 * registered scene with the specified unique identifier.
 *
 * This function removes all of the currently drawn scenes from the screen,
 * leaving only the registered scene with the specified unique identifier.
 *
 * @param uid The unique identifier of the scene to leave on the screen.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was switched to successfully, or `-1` if an
 * error occurred.
 */
int Scene_switchToRegistered(int uid, int x, int y);

/**
 * @brief Scrolls the scene by the specified amount.
 *
 * This function scrolls the scene by the specified amount. The amount is
 * specified by the `dx` and `dy` parameters, which represent the amount to
 * scroll in the x and y directions, respectively.
 *
 * Scrolling refers to moving (redrawing) the specified draw bounds over by
 * the values of `dx` and `dy`. This is useful for creating scrolling
 * effects in the scene.
 *
 * @param uid The unique identifier of the scene to scroll.
 * @param dx The amount to scroll in the x direction.
 * @param dy The amount to scroll in the y direction.
 * @return `0` if the scene was scrolled successfully, `-1` if an error
 * occurred.
 */
int Scene_scroll(int uid, int dx, int dy);

/**
 * @brief Ticks the CmdFX Scene Engine.
 *
 * This function calls all of the necessary things apart of the Scene Engine
 * to keep scenes redrawn every frame.
 */
void tickCmdFXSceneEngine();

/**
 * @brief Begins the CmdFX Scene Engine.
 *
 * This function initializes the CmdFX Scene Engine. The Scene Engine is
 * responsible for managing all scenes that are drawn on the screen. It will
 * automatically redraw scenes each frame if necessary for registered
 * scenes.
 *
 * @return `0` if the Scene Engine was started successfully, or `-1` if an
 * error occurred.
 */
int beginCmdFXSceneEngine();

/**
 * @brief Ends the CmdFX Scene Engine.
 *
 * This function stops the CmdFX Scene Engine. The Scene Engine will no
 * longer manage scenes that are drawn on the screen. Scenes will no longer
 * be automatically redrawn each frame.
 *
 * @return `0` if the Scene Engine was stopped successfully, or `-1` if an
 * error occurred.
 */
int endCmdFXSceneEngine();

// Scene Data Manipulation

/**
 * @brief Sets the data of the scene.
 *
 * This function sets the data of the scene to the specified 2D array of
 * characters. The width and height of the scene will be set to the width
 * and height of the 2D array. The data field will be resized to match the
 * new size. If an error occurs, the function will return `-1`.
 *
 * The scene will be redrawn if it is currently drawn.
 *
 * @param scene The scene to modify.
 * @param data The 2D array of characters to set.
 * @return `0` if the scene data was set, or `-1` if an error occurred.
 */
int Scene_setData(CmdFX_Scene* scene, char** data);

/**
 * @brief Sets the character at the specified position in the scene.
 *
 * This function sets the character at the specified position in the scene.
 * The position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the scene. If the position is out of bounds, the
 * function will return
 * `-1`.
 *
 * If the scene has no data, this function will return `-1`.
 *
 * This function will also redraw the character if the scene is currently
 * drawn.
 *
 * @param scene The scene to modify.
 * @param x The X position in the scene.
 * @param y The Y position in the scene.
 * @param c The character to set.
 * @return `0` if the character was set, or `-1` if the position is out of
 * bounds.
 */
int Scene_setChar(CmdFX_Scene* scene, int x, int y, char c);

/**
 * @brief Sets the ANSI data of the scene.
 *
 * This function sets the ANSI data to the current ANSI data of the scene.
 * The width and height of the scene must match the width and height of the
 * 2D array. The ANSI data field will be resized to match the new size. If
 * an error occurs, the function will return `-1`.
 *
 * The scene will be redrawn if it is currently drawn.
 *
 * @param scene The scene to modify.
 * @param ansiData The 2D array of ANSI strings to set.
 * @return `0` if the scene data was set, or `-1` if an error occurred.
 */
int Scene_setAnsiData(CmdFX_Scene* scene, char*** ansiData);

/**
 * @brief Appends the ANSi data to the current ansi data, setting it if it
 * is not currently set.
 *
 * This function appends the ANSI data to the current ANSI data of the
 * scene. The width and height of the scene must match the width and height
 * of the 2D array. The ANSI data field will be resized to match the new
 * size. If an error occurs, the function will return `-1`.
 *
 * The scene will be redrawn if it is currently drawn.
 *
 * @param scene The scene to modify.
 * @param ansiData The 2D array of ANSI strings to append.
 * @return `0` if the scene data was set, or `-1` if an error occurred.
 */
int Scene_appendAnsiData(CmdFX_Scene* scene, char*** ansiData);

/**
 * @brief Fills the foreground of the scene with the specified RGB color.
 *
 * This function fills the foreground of the scene with the specified RGB
 * color. The position is specified by the X and Y coordinates, where (0, 0)
 * is the top-left corner of the scene. If the position is out of bounds,
 * the function will return `-1`.
 *
 * @param scene The scene to modify.
 * @param x The top-left X position in the scene.
 * @param y The top-left Y position in the scene.
 * @param width The width of the fill.
 * @param height The height of the fill.
 * @param rgb The RGB color to set.
 * @return `0` if the fill was successful, or `-1` if an error occurred.
 */
int Scene_setForeground(
    CmdFX_Scene* scene, int x, int y, int width, int height, int rgb
);

/**
 * @brief Fills the entirety of the foreground of the scene with the
 * specified RGB color.
 *
 * This function fills the entirety of the foreground of the scene with the
 * specified RGB color. If an error occurs, the function will return `-1`.
 *
 * @param scene The scene to modify.
 * @param rgb The RGB color to set.
 * @return `0` if the fill was successful, or `-1` if an error occurred.
 */
int Scene_setForegroundAll(CmdFX_Scene* scene, int rgb);

/**
 * @brief Fills the background of the scene with the specified RGB color.
 *
 * This function fills the background of the scene with the specified RGB
 * color. The position is specified by the X and Y coordinates, where (0, 0)
 * is the top-left corner of the scene. If the position is out of bounds,
 * the function will return `-1`.
 *
 * @param scene The scene to modify.
 * @param x The top-left X position in the scene.
 * @param y The top-left Y position in the scene.
 * @param width The width of the fill.
 * @param height The height of the fill.
 * @param rgb The RGB color to set.
 * @return `0` if the fill was successful, or `-1` if an error occurred.
 */
int Scene_setBackground(
    CmdFX_Scene* scene, int x, int y, int width, int height, int rgb
);

/**
 * @brief Fills the entirety of the background of the scene with the
 * specified RGB color.
 *
 * This function fills the entirety of the background of the scene with the
 * specified RGB color. If an error occurs, the function will return `-1`.
 *
 * @param scene The scene to modify.
 * @param rgb The RGB color to set.
 * @return `0` if the fill was successful, or `-1` if an error occurred.
 */
int Scene_setBackgroundAll(CmdFX_Scene* scene, int rgb);

#ifdef __cplusplus
}
#endif