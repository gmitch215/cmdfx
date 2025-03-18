/**
 * @file util.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Physics utilities for cmdfx
 * @version 0.1.1
 * @date 2025-02-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Vectors

/**
 * @brief A 2D Vector.
 */
typedef struct CmdFX_Vector {
    /**
     * @brief The x coordinate.
     */
    int x;
    /**
     * @brief The y coordinate.
     */
    int y;
} CmdFX_Vector;

/**
 * @brief Creates a new 2D Vector.
 * 
 * This method creates a new 2D Vector with the given x and y coordinates.
 * The vector must be freed with `free` when it is no longer needed.
 * 
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The new 2D Vector.
 */
CmdFX_Vector* Vector_create(int x, int y);

/**
 * @brief Adds two 2D Vectors.
 * 
 * @param v1 The first 2D Vector.
 * @param v2 The second 2D Vector.
 * @return The sum of the two 2D Vectors.
 */
CmdFX_Vector* Vector_add(CmdFX_Vector* v1, CmdFX_Vector* v2);

/**
 * @brief Adds multiple 2D Vectors.
 * 
 * @param vectors The 2D Vectors.
 * @param count The number of 2D Vectors.
 * @return The sum of the 2D Vectors.
 */
CmdFX_Vector* Vector_addAll(CmdFX_Vector** vectors, int count);

/**
 * @brief Subtracts two 2D Vectors.
 * 
 * @param v1 The first 2D Vector.
 * @param v2 The second 2D Vector.
 * @return The difference of the two 2D Vectors.
 */
CmdFX_Vector* Vector_subtract(CmdFX_Vector* v1, CmdFX_Vector* v2);

/**
 * @brief Multiplies a 2D Vector by a scalar.
 * 
 * @param v The 2D Vector.
 * @param scalar The scalar.
 * @return 0 if successful, -1 if an error occurred.
 */
int Vector_multiply(CmdFX_Vector* v, double scalar);

/**
 * @brief Divides a 2D Vector by a scalar.
 * 
 * @param v The 2D Vector.
 * @param scalar The scalar.
 * @return 0 if successful, -1 if an error occurred.
 */
int Vector_divide(CmdFX_Vector* v, double scalar);

/**
 * @brief Rotates a 2D Vector by a specific number of radians.
 * 
 * This method rotates a 2D Vector by a specific number of radians.
 * The rotation will be done around the origin.
 * 
 * @param v The 2D Vector.
 * @param radians The number of radians to rotate.
 * @return 0 if successful, -1 if an error occurred.
 */
int Vector_rotate(CmdFX_Vector* v, double radians);

/**
 * @brief Flips a 2D Vector horizontally.
 * 
 * @param v The 2D Vector.
 * @return 0 if successful, -1 if an error occurred.
 */
int Vector_flipX(CmdFX_Vector* v);

/**
 * @brief Flips a 2D Vector vertically.
 * 
 * @param v The 2D Vector.
 * @return 0 if successful, -1 if an error occurred.
 */
int Vector_flipY(CmdFX_Vector* v);

/**
 * @brief Flips a 2D Vector.
 * 
 * @param v The 2D Vector.
 * @return 0 if successful, -1 if an error occurred.
 */
int Vector_flip(CmdFX_Vector* v);

/**
 * @brief Gets the dot product of two vectors.
 * @param v1 The first 2D Vector.
 * @param v2 The second 2D Vector.
 * @return The dot product.
 */
int Vector_dot(CmdFX_Vector* v1, CmdFX_Vector* v2);

/**
 * @brief Gets the magnitude of a 2D Vector.
 * 
 * @param v The 2D Vector.
 * @return The magnitude of the 2D Vector.
 */
double Vector_getMagnitude(CmdFX_Vector* v);

/**
 * @brief Gets the angle of a 2D Vector.
 * 
 * @param v The 2D Vector.
 * @return The angle of the 2D Vector, in radians.
 */
double Vector_getAngle(CmdFX_Vector* v);

#ifdef __cplusplus
}
#endif