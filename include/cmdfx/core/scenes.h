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
 * The maximum number of registered scenes that can be stored is 256. This is the maximum
 * number of scenes that can be registered with the Scene Engine at any given time.
 */
#define MAX_REGISTERED_SCENES 1024

/**
 * @brief Represents a CmdFX Scene.
 * 
 * A CmdFX Scene is a storage buffer for different characters on the screen. It is
 * drawn using the cnvas API and should be used when displaying large amounts of
 * characters at a time.
 */
typedef struct CmdFX_Scene {
    /**
     * @brief The unique identifier for the scene.
     * 
     * The unique identifier is a number that is assigned to the scene when it is
     * created. This number is used to identify the scene when it is registered to
     * the Scene Engine.
     * 
     * If this number is `-1`, the scene has not been registered to the Scene Engine.
     */
    int uid;
    /**
     * @brief Represents the width of the scene.
     * 
     * The width of the scene is the number of characters that can be displayed
     * horizontally. This is the same as the width of each array in the data field.
     */
    int width;
    /**
     * @brief Represents the height of the scene.
     * 
     * The height of the scene is the number of characters that can be displayed
     * vertically. This is the same as the number of arrays in the data field.  
     */
    int height;
    /**
     * @brief Represents the data of the scene.
     * 
     * The data field is a 2D array of characters that represent the scene. Each
     * array in the data field represents a row of characters on the screen. The
     * width of the scene is the same as the width of each array in the data field.
     * The height of the scene is the same as the number of arrays in the data field.
     * 
     * These characters are drawn using the Canvas API and will therefore disappear
     * if a Sprite is drawn over them. The Scene Engine will automatically redraw the
     * scene each frame if necessary.
     */
    char** data;
    /**
     * @brief Represents the ANSI data of the scene.
     * 
     * The ANSI data field is a 2D array of ANSI characters that represent the scene.
     * Each array in the ANSI data field represents a row of characters on the screen.
     * The width of the scene is the same as the width of each array in the ANSI data
     * field. The height of the scene is the same as the number of arrays in the ANSI
     * data field.
     * 
     * These characters are drawn using the Canvas API and will therefore disappear if
     * a Sprite is drawn over them. The Scene Engine will automatically redraw the scene
     * each frame if necessary.
     */
    char*** ansiData;
    /**
     * @brief Gets the x-coordinate of the scene.
     * 
     * The x-coordinate of the scene is the position of the top-left corner of the scene
     * on the screen. This is the same as the x-coordinate of the scene when it is drawn
     * using the `Scene_draw` function. If the scene is not currently drawn, it will be
     * `-1`.
     */
    int x;
    /**
     * @brief Gets the y-coordinate of the scene.
     * 
     * The y-coordinate of the scene is the position of the top-left corner of the scene
     * on the screen. This is the same as the y-coordinate of the scene when it is drawn
     * using the `Scene_draw` function. If the scene is not currently drawn, it will be
     * `-1`.
     */
    int y;
    /**
     * @brief Represents the Z index of the scene.
     * 
     * The Z index of the scene is the order in which the scene is drawn on the screen.
     * Scenes with a higher Z index will be drawn on top of scenes with a lower Z index.
     */
    int z;
} CmdFX_Scene;

/**
 * @brief Gets a list of all scenes that are drawn on the screen.
 * @return An array of pointers to all scenes that are drawn on the screen.
 */
CmdFX_Scene** Scene_getDrawnScenes();

/**
 * @brief Gets the number of scenes that are drawn on the screen.
 * @return The number of scenes that are drawn on the screen.
 */
int Scene_getDrawnScenesCount();

/**
 * @brief Creates an empty scene with a specified width and height.
 * 
 * This function creates a new scene with the specified width and height. The scene
 * will be empty, meaning that all characters in the data field will be set to `' '`.
 * 
 * The width and height parameters must be greater than 0. If either parameter is less
 * than or equal to 0, the function will return `NULL`.
 * 
 * @param width The width of the scene.
 * @param height The height of the scene.
 * @return A pointer to the newly created scene, or `NULL` if the scene could not be created.
 */
CmdFX_Scene* Scene_create(int width, int height);

/**
 * @brief Creates a scene from a 2D array of characters and ANSI strings.
 * 
 * This function creates a new scene from the specified 2D array of characters and ANSI
 * strings. The width and height of the scene will be set to the width and height of the
 * 2D array. Therefore, the arrays must be properly null-teminated with a `NULL` character
 * at the end of each array.
 * 
 * The bounds will be based on the `data` array. If the bounds do not match, the function
 * will return `NULL`.
 * 
 * The data parameter must not be `NULL`. If data is `NULL`, the function will return `NULL`.
 * `ansiData` can optionally be `NULL` to represent no color.
 * 
 * @param data The 2D array of characters that represent the scene.
 * @param ansiData The 2D array of ANSI strings that represent the scene.
 * @return A pointer to the newly created scene, or `NULL` if the scene could not be created.
 */
CmdFX_Scene* Scene_createFromData(char** data, char*** ansiData);

/**
 * @brief Clears a scene of its data.
 * 
 * This function clears the specified scene of its data. This means that all characters in
 * the data field will be set to 0, making `Scene_isEmpty` return `1`.
 * 
 * @param scene The scene to clear.
 * @return `0` if the scene was cleared successfully, or `-1` if an error occurred.
 */
int Scene_clear(CmdFX_Scene* scene);

/**
 * @brief Draws a scene at the specified position on the screen.
 * 
 * This function draws the specified scene at the specified position on the screen. The
 * scene will be drawn using the Canvas API, which means that it will disappear if a Sprite
 * is drawn over it. The Scene Engine will automatically redraw the scene each frame if
 * necessary.
 * 
 * @param scene The scene to draw. 
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was drawn successfully, or `-1` if an error occurred.
 */
int Scene_draw(CmdFX_Scene* scene, int x, int y);

/**
 * @brief Gets the scene at the specified position on the screen.
 * 
 * This function gets the top-most scene at the specified position on the screen. 
 * The scene will be drawn using the Canvas API, which means that it will disappear if 
 * a Sprite is drawn over it. The Scene Engine will automatically redraw the scene each 
 * frame if necessary.
 * 
 * The x and y parameters must be greater than or equal to 0. If either parameter is
 * less than 0, the function will return `NULL`.
 * 
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return A pointer to the scene at the specified position, or `NULL` if no scene was found.
 */
CmdFX_Scene* Scene_getSceneAt(int x, int y);

/**
 * @brief Checks if the specified scene is on top of the screen.
 * 
 * This function checks if the specified scene is on top of the screen. A scene is
 * considered to be on top if it is the top-most scene at its position on the screen
 * according to `scene->z`.
 * 
 * @param scene The scene to check.
 * @return `1` if the scene is on top, or `0` if the scene is not on top.
 */
int Scene_isOnTop(CmdFX_Scene* scene);

/**
 * @brief Checks if the specified scene is on top at the specified position on the screen.
 * 
 * This function checks if the specified scene is on top at the specified position on the
 * screen. A scene is considered to be on top if it is the top-most scene at its position
 * on the screen according to `scene->z`.
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
 * This function checks if the specified scene is on the bottom of the screen. A scene is
 * considered to be on the bottom if it is the bottom-most scene at its position on the
 * screen according to `scene->z`.
 * 
 * @param scene The scene to check.
 * @return `1` if the scene is on the bottom, or `0` if the scene is not on bottom.
 */
int Scene_isOnBottom(CmdFX_Scene* scene);

/**
 * @brief Checks if the specified scene is on the bottom at the specified position on the screen.
 * 
 * This function checks if the specified scene is on the bottom at the specified position
 * on the screen. A scene is considered to be on the bottom if it is the bottom-most scene
 * at its position on the screen according to `scene->z`.
 * 
 * @param scene The scene to check.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `1` if the scene is on bottom, or `0` if the scene is not on bottom.
 */
int Scene_isOnBottomAt(CmdFX_Scene* scene, int x, int y);

/**
 * @brief Removes a scene from the screen.
 * 
 * This function removes the specified scene from the screen. The scene will no longer be
 * drawn on the screen, and the Scene Engine will stop automatically redrawing it each frame.
 * 
 * @param scene The scene to remove.
 * @return `0` if the scene was removed successfully, or `-1` if an error occurred.
 */
int Scene_remove(CmdFX_Scene* scene);

/**
 * @brief Checks if the specified scene is empty.
 * 
 * This function checks if the specified scene is empty. A scene is considered empty if all
 * characters in the data field are set to 0.
 * 
 * @param scene The scene to check.
 * @return `1` if the scene is empty, or `0` if the scene is not empty.
 */
int Scene_isEmpty(CmdFX_Scene* scene);

/**
 * @brief Removes all of the currently drawn scenes, only leaving this scene.
 * 
 * This function removes all of the currently drawn scenes from the screen, leaving only
 * the specified scene.
 * 
 * @param scene The scene to leave on the screen.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was switched to successfully, or `-1` if an error occurred.
 */
int Scene_switchTo(CmdFX_Scene* scene, int x, int y);

/**
 * @brief Destroys the scene, freeing the memory allocated.
 * 
 * This function frees the memory allocated for the specified scene. After calling this
 * function, the scene should no longer be used.
 * 
 * @param scene The scene to free.
 * @return `0` if the scene was freed successfully, or `-1` if an error occurred.
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
 * @brief Registers a scene with the Scene Engine.
 * 
 * This function registers the specified scene with the Scene Engine. This allows the Scene
 * Engine to keep track of the scene and automatically redraw it each frame if necessary.
 * 
 * @param scene The scene to register.
 * @return `scene->uid`, or `-1` if an error occurred.
 */
int Scene_register(CmdFX_Scene* scene);

/**
 * @brief Unregisters a scene with the Scene Engine.
 * 
 * This function unregisters the specified scene with the Scene Engine. This allows the Scene
 * Engine to stop keeping track of the scene and stop automatically redrawing it each frame.
 * 
 * @param scene The scene to unregister.
 * @return the old `scene->uid`, or `-1` if an error occurred.
 */
int Scene_unregister(CmdFX_Scene* scene);

/**
 * @brief Draws a registered scene at the specified position on the screen.
 * 
 * This function draws the registered scene with the specified unique identifier at the
 * specified position on the screen. The scene will be drawn using the Canvas API, which
 * means that it will disappear if a Sprite is drawn over it. The Scene Engine will
 * automatically redraw the scene each frame if necessary.
 * 
 * @param uid The unique identifier of the scene to draw.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was drawn successfully, or `-1` if an error occurred.
 */
int Scene_drawRegistered(int uid, int x, int y);

/**
 * @brief Removes all of the currently drawn scenes, only leaving the registered scene with the specified unique identifier.
 * 
 * This function removes all of the currently drawn scenes from the screen, leaving only the
 * registered scene with the specified unique identifier.
 * 
 * @param uid The unique identifier of the scene to leave on the screen.
 * @param x The x-coordinate of the top-left corner of the scene.
 * @param y The y-coordinate of the top-left corner of the scene.
 * @return `0` if the scene was switched to successfully, or `-1` if an error occurred.
 */
int Scene_switchToRegistered(int uid, int x, int y);

/**
 * @brief Ticks the CmdFX Scene Engine.
 * 
 * This function calls all of the necessary things apart of the Scene Engine to keep
 * scenes redrawn every frame.
 */
void tickCmdFXSceneEngine();

/**
 * @brief Begins the CmdFX Scene Engine.
 * 
 * This function initializes the CmdFX Scene Engine. The Scene Engine is responsible for
 * managing all scenes that are drawn on the screen. It will automatically redraw scenes
 * each frame if necessary for registered scenes.
 * 
 * @return `0` if the Scene Engine was started successfully, or `-1` if an error occurred.
 */
int beginCmdFXSceneEngine();

/**
 * @brief Ends the CmdFX Scene Engine.
 * 
 * This function stops the CmdFX Scene Engine. The Scene Engine will no longer manage scenes
 * that are drawn on the screen. Scenes will no longer be automatically redrawn each frame.
 * 
 * @return `0` if the Scene Engine was stopped successfully, or `-1` if an error occurred.
 */
int endCmdFXSceneEngine();

#ifdef __cplusplus
}
#endif