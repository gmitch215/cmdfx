#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/physics/engine.h"

// Engine Declarations

int _forceOfGravity = 1;

int Engine_getForceOfGravity() {
    return _forceOfGravity;
}

int Engine_setForceOfGravity(int force) {
    if (force < 0) return -1;

    _forceOfGravity = force;

    return 0;
}

int _terminalVelocity = 10;

int Engine_getTerminalVelocity() {
    return _terminalVelocity;
}

int Engine_setTerminalVelocity(int velocity) {
    if (velocity < 1) return -1;

    _terminalVelocity = velocity;

    return 0;
}

int _groundY = -1;

int Engine_getGroundY() {
    if (_groundY == -1) return Canvas_getHeight();

    return _groundY;
}

int Engine_setGroundY(int y) {
    if (y < 0) return -1;

    _groundY = y;

    return 0;
}

// Engine Defaults

double _defaultFrictionCoefficient = 0.25;

double Engine_getDefaultFrictionCoefficient() {
    return _defaultFrictionCoefficient;
}

int Engine_setDefaultFrictionCoefficient(double coefficient) {
    if (coefficient < 0 || coefficient > 1) return -1;

    _defaultFrictionCoefficient = coefficient;

    return 0;
}

int* _characterMasses = 0;

int Engine_getCharacterMass(char c) {
    if (_characterMasses == 0) return 1;

    return _characterMasses[(int) c] + 1;
}

int Engine_setCharacterMass(char c, int mass) {
    if (mass < 1) return -1;
    
    if (_characterMasses == 0) {
        _characterMasses = calloc(256, sizeof(int));
        if (_characterMasses == 0) return -1;
    }

    _characterMasses[(int) c] = mass - 1;

    return 0;
}

// Engine

int Engine_cleanup() {
    // cleanup loose variables
    if (_characterMasses != 0) free(_characterMasses);

    return 0;
}

void Engine_tick() {
    // TODO
}