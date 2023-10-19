#include "keybinds.h"
#include "logger.h"

#include <SDL2/SDL_keycode.h>

static Keybind keybinds[] = {
    {SDLK_LCTRL, false},
    {SDLK_LSHIFT, false}
};
static const unsigned int keybinds_len = sizeof(keybinds) / sizeof(keybinds[0]);

bool keybinds_init(void) { return true; }
void keybinds_destroy(void) { }

void keybinds_on_up(int keycode) {
    for (unsigned int i = 0; i < keybinds_len; ++i) {
        if (keybinds[i].keycode == keycode) {
            keybinds[i].pressed = false;
        }
    }
}

void keybinds_on_down(int keycode) {
    for (unsigned int i = 0; i < keybinds_len; ++i) {
        if (keybinds[i].keycode == keycode) {
            keybinds[i].pressed = true;
        }
    }
}

bool keybinds_is_down(int keycode) {
    for (unsigned int i = 0; i < keybinds_len; ++i) {
        if (keybinds[i].keycode == keycode) {
            return keybinds[i].pressed;
        }
    }
    return false;
}
