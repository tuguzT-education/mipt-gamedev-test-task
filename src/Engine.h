#pragma once

#include <cstdint>

namespace engine {
    constexpr size_t screen_width = 1024u;
    constexpr size_t screen_height = 768u;

    struct color {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t unused;
    };

    extern color buffer[screen_height][screen_width];

    enum class virtual_key : uint8_t {
        escape,
        space,
        left,
        up,
        right,
        down,
    };

    enum class mouse_button : uint8_t {
        left,
        right,
    };

    using cursor_coordinate = int;
    using seconds = float;

    bool is_key_pressed(virtual_key key);

    bool is_mouse_button_pressed(mouse_button button);

    cursor_coordinate get_cursor_x();

    cursor_coordinate get_cursor_y();

    void initialize();

    void act(seconds delta_time);

    void draw();

    void schedule_quit();

    void finalize();
}
