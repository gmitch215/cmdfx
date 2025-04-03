#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmdfx/physics/util.h"

// Vectors

CmdFX_Vector* Vector_create(double x, double y) {
    CmdFX_Vector* v = malloc(sizeof(CmdFX_Vector));
    if (v == 0) return 0;

    v->x = x;
    v->y = y;

    return v;
}

CmdFX_Vector* Vector_add(CmdFX_Vector* v1, CmdFX_Vector* v2) {
    CmdFX_Vector* v = malloc(sizeof(CmdFX_Vector));
    if (v == 0) return 0;

    if (v1 == 0 && v2 == 0) {
        v->x = 0.0;
        v->y = 0.0;
        return v;
    }

    if (v1 == 0) {
        v->x = v2->x;
        v->y = v2->y;
        return v;
    }

    if (v2 == 0) {
        v->x = v1->x;
        v->y = v1->y;
        return v;
    }

    v->x = v1->x + v2->x;
    v->y = v1->y + v2->y;

    return v;
}

CmdFX_Vector* Vector_addAll(CmdFX_Vector** vectors, int count) {
    CmdFX_Vector* v = malloc(sizeof(CmdFX_Vector));
    if (v == 0) return 0;

    v->x = 0.0;
    v->y = 0.0;

    if (vectors == 0) return v;
    if (count <= 0) return v;

    for (int i = 0; i < count; i++) {
        if (vectors[i] == 0) continue;

        v->x += vectors[i]->x;
        v->y += vectors[i]->y;
    }

    return v;
}

CmdFX_Vector* Vector_subtract(CmdFX_Vector* v1, CmdFX_Vector* v2) {
    CmdFX_Vector* v = malloc(sizeof(CmdFX_Vector));
    if (v == 0) return 0;

    if (v1 == 0 && v2 == 0) {
        v->x = 0;
        v->y = 0;
        return v;
    }

    if (v1 == 0) {
        v->x = -v2->x;
        v->y = -v2->y;
        return v;
    }

    if (v2 == 0) {
        v->x = v1->x;
        v->y = v1->y;
        return v;
    }

    v->x = v1->x - v2->x;
    v->y = v1->y - v2->y;

    return v;
}

int Vector_multiply(CmdFX_Vector* v, double scalar) {
    if (v == 0) return -1;

    v->x *= scalar;
    v->y *= scalar;

    return 0;
}

int Vector_divide(CmdFX_Vector* v, double scalar) {
    if (v == 0) return -1;
    if (scalar == 0.0) return -1;

    v->x /= scalar;
    v->y /= scalar;

    return 0;
}

int Vector_rotate(CmdFX_Vector* v, double radians) {
    if (v == 0) return -1;

    double x = v->x;
    double y = v->y;

    v->x = (x * cos(radians)) - (y * sin(radians));
    v->y = (x * sin(radians)) + (y * cos(radians));

    return 0;
}

int Vector_flipX(CmdFX_Vector* v) {
    if (v == 0) return -1;

    v->x = -v->x;
    return 0;
}

int Vector_flipY(CmdFX_Vector* v) {
    if (v == 0) return -1;

    v->y = -v->y;
    return 0;
}

int Vector_flip(CmdFX_Vector* v) {
    if (v == 0) return -1;

    v->x = -v->x;
    v->y = -v->y;
    return 0;
}

int Vector_dot(CmdFX_Vector* v1, CmdFX_Vector* v2) {
    if (v1 == 0 || v2 == 0) return 0;
    if (v1->x == 0 && v1->y == 0) return 0;
    if (v2->x == 0 && v2->y == 0) return 0;

    return (v1->x * v2->x) + (v1->y * v2->y);
}

double Vector_getMagnitude(CmdFX_Vector* v) {
    if (v == 0) return 0.0;
    if (v->x == 0 && v->y == 0) return 0.0;

    return sqrt((v->x * v->x) + (v->y * v->y));
}

double Vector_getAngle(CmdFX_Vector* v) {
    if (v == 0) return 0.0;
    if (v->x == 0 && v->y == 0) return 0.0;

    return atan2(v->y, v->x);
}