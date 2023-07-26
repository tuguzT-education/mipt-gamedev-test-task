#include "Engine.h"

#include <memory>

//  is_key_pressed(virtual_key key) - check if a key is pressed
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(mouse_button button) - check if mouse button is pressed
//  schedule_quit() - quit game after act()

// initialize game data in this function
void engine::initialize() {
}

// this function is called to update game data,
// delta_time - time elapsed since the previous update (in seconds)
void engine::act(seconds delta_time) {
    if (is_key_pressed(virtual_key::escape))
        schedule_quit();
}

// fill buffer in this function
void engine::draw() {
    // clear buffer (set all pixels to black)
    memset(buffer, 0, sizeof(buffer));
}

// free game data in this function
void engine::finalize() {
}
