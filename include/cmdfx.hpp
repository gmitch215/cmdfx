/**
 * @file cmdfx.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extension Header for CmdFX
 * @version 0.2.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include "cmdfx.h"

/**
 * @brief Primary namespace for CmdFX.
 * 
 * This is the main namespace for the CmdFX library. All classes and functions
 * are defined within this namespace. The CmdFX library is designed to provide
 * a simple and easy-to-use interface for creating graphics and animations in
 * the terminal.
 */
namespace CmdFX {}

// Core Library

#include "cmdfx/core/canvas.hpp"
#include "cmdfx/core/builder.hpp"
#include "cmdfx/core/costumes.hpp"
#include "cmdfx/core/device.hpp"
#include "cmdfx/core/sprites.hpp"
#include "cmdfx/core/screen.hpp"
#include "cmdfx/core/scenes.hpp"
#include "cmdfx/core/util.hpp"

// Physics Engine

#include "cmdfx/physics/engine.hpp"
#include "cmdfx/physics/force.hpp"
#include "cmdfx/physics/mass.hpp"
#include "cmdfx/physics/motion.hpp"
#include "cmdfx/physics/util.hpp"

// UI Library

#include "cmdfx/ui/button.hpp"
#include "cmdfx/ui/path.hpp"
#include "cmdfx/ui/scenes.hpp"
#include "cmdfx/ui/switch.hpp"