#ifndef _KEYBINDS_H_
#define _KEYBINDS_H_

#include <stdbool.h>

typedef struct _Keybind_ {
    int keycode;
    bool pressed;
} Keybind;

bool keybinds_init(void);
void keybinds_destroy(void);

void keybinds_on_up(int keycode);
void keybinds_on_down(int keycode);

bool keybinds_is_down(int keycode);

#endif // !_KEYBINDS_H_
