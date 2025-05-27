/**
 * @file util.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ extensions for the CmdFX Physics Utilities API 
 * @version 0.2.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

extern "C" {
    #include <stdlib.h>
    #include "cmdfx/physics/util.h"
}

#include <vector>

namespace CmdFX {
    /**
     * @brief A C++ wrapper around a CmdFX_Vector struct.
     * 
     * This class is a wrapper around the CmdFX_Vector struct. It provides a
     * constructor and destructor for the vector, as well as methods to get and set
     * the vector's coordinates. The destructor will free the vector's memory when the
     * object is destroyed.
     */
    class Vector final {
        private:
            CmdFX_Vector* vector;

        public:
            Vector(CmdFX_Vector* vector) : vector(vector) {}
            Vector(double x, double y) {
                vector = Vector_create(x, y);
            }

            ~Vector() {
                if (vector) { free(vector); }
            }

            CmdFX_Vector* getVector() { return vector; }

            double getX() const { return vector->x; }
            double getY() const { return vector->y; }
            double getMagnitude() const { return Vector_getMagnitude(vector); }

            void setX(double x) { vector->x = x; }
            void setY(double y) { vector->y = y; }

            Vector operator+(const Vector& other) const {
                CmdFX_Vector* result = Vector_add(vector, other.vector);
                return Vector(result);
            }

            Vector operator+(const std::vector<Vector>& vectors) const {
                CmdFX_Vector** vArray = new CmdFX_Vector*[vectors.size() + 1];
                vArray[0] = vector;

                for (size_t i = 0; i < vectors.size(); ++i) {
                    vArray[i + 1] = vectors[i].vector;
                }

                CmdFX_Vector* result = Vector_addAll(vArray, vectors.size() + 1);
                delete[] vArray;

                return Vector(result);
            }

            Vector& operator+=(const Vector& other) {
                CmdFX_Vector* result = Vector_add(vector, other.vector);
                free(vector);
                vector = result;
                
                return *this;
            }

            Vector operator-(const Vector& other) const {
                CmdFX_Vector* result = Vector_subtract(vector, other.vector);
                return Vector(result);
            }

            Vector& operator-=(const Vector& other) {
                CmdFX_Vector* result = Vector_subtract(vector, other.vector);
                free(vector);
                vector = result;
                
                return *this;
            }

            Vector operator*(double scalar) const {
                Vector result(vector->x, vector->y);
                Vector_multiply(result.vector, scalar);
                return result;
            }

            Vector& operator*=(double scalar) {
                Vector_multiply(vector, scalar);
                return *this;
            }

            Vector operator/(double scalar) const {
                Vector result(vector->x, vector->y);
                Vector_divide(result.vector, scalar);
                return result;
            }

            Vector& operator/=(double scalar) {
                Vector_divide(vector, scalar);
                return *this;
            }

            int rotate(double radians) {
                return Vector_rotate(vector, radians);
            }

            double getAngle() {
                return Vector_getAngle(vector);
            }

            int flipX() {
                return Vector_flipX(vector);
            }

            int flipY() {
                return Vector_flipY(vector);
            }

            int flip() {
                return Vector_flip(vector);
            }

            int dot(const Vector& other) const {
                return Vector_dot(vector, other.vector);
            }
    };
}