#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/costumes.h"
#include "cmdfx/ui/button.h"
#include "cmdfx/ui/switch.h"

CmdFX_Button* Button_createSwitch(
    CmdFX_Sprite* sprite, CmdFX_ButtonCallback callback, bool state
) {
    CmdFX_Button* button = Button_create(sprite, callback);
    if (button == 0) return 0;

    button->extra = malloc(sizeof(bool));
    if (button->extra == 0) {
        free(button);
        return 0;
    }
    *(bool*) (button->extra) = state;
    button->type = CMDFX_BUTTON_TYPE_SWITCH;

    Sprite_createCostumes(button->sprite, 2);

    return button;
}

CmdFX_Button* Button_createSwitchWith(
    char** on, char** off, char*** ansiOn, char*** ansiOff,
    CmdFX_ButtonCallback callback, bool state
) {
    if (on == 0 || off == 0) return 0;

    CmdFX_Button* button = Button_createSwitch(0, callback, state);
    if (button == 0) return 0;

    Sprite_setCostumeAt(button->sprite, 0, on, ansiOn);
    Sprite_setCostumeAt(button->sprite, 1, off, ansiOff);
    Sprite_switchCostumeTo(button->sprite, state ? 1 : 0);

    return button;
}

bool Switch_getState(CmdFX_Button* button) {
    if (button == 0) return false;
    if (button->extra == 0) return false;
    if (button->type != CMDFX_BUTTON_TYPE_SWITCH) return false;

    return *(bool*) (button->extra);
}

int Switch_setState(CmdFX_Button* button, bool state) {
    if (button == 0) return -1;
    if (button->extra == 0) return -1;
    if (button->type != CMDFX_BUTTON_TYPE_SWITCH) return -1;

    *(bool*) (button->extra) = state;
    Sprite_switchCostumeTo(button->sprite, state ? 1 : 0);

    return 0;
}

int Switch_toggleState(CmdFX_Button* button) {
    if (button == 0) return -1;
    if (button->extra == 0) return -1;
    if (button->type != CMDFX_BUTTON_TYPE_SWITCH) return -1;

    *(bool*) (button->extra) = !(*(bool*) (button->extra));
    Sprite_switchCostumeTo(button->sprite, *(bool*) (button->extra) ? 1 : 0);

    return 0;
}