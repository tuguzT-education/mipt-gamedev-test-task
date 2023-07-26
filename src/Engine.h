#pragma once

#include <cstdint>

namespace engine {
    using screen_coordinate = std::uint16_t;
    using seconds = float;

    constexpr screen_coordinate screen_width = 1024u;
    constexpr screen_coordinate screen_height = 768u;

    struct color final {
        std::uint8_t blue;
        std::uint8_t green;
        std::uint8_t red;
        std::uint8_t alpha;
    };

    extern color buffer[screen_height][screen_width];

    enum class virtual_key : std::uint8_t {
        escape,
        space,
        left,
        up,
        right,
        down,
    };

    enum class mouse_button : std::uint8_t {
        left,
        right,
    };

    bool is_key_pressed(virtual_key key);

    bool is_mouse_button_pressed(mouse_button button);

    screen_coordinate get_cursor_x();

    screen_coordinate get_cursor_y();

    void initialize();

    void act(seconds delta_time);

    void draw();

    void schedule_quit();

    void finalize();
}
