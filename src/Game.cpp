#include "Game.h"

#include <memory>

//  is_key_pressed(virtual_key key) - check if a key is pressed
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(mouse_button button) - check if mouse button is pressed
//  schedule_quit() - quit game after act()
namespace engine {
    // initialize game data in this function
    void initialize() {
    }

    // this function is called to update game data
    // delta_time - time elapsed since the previous update (in seconds)
    void act(seconds delta_time) {
        if (is_key_pressed(virtual_key::escape))
            schedule_quit();

        const auto vertical = is_key_pressed(virtual_key::down) - is_key_pressed(virtual_key::up);
        const auto horizontal = is_key_pressed(virtual_key::right) - is_key_pressed(virtual_key::left);

        auto &player = game::current_player;
        player.position.y += player.speed * static_cast<game::coordinate>(vertical) * delta_time;
        player.position.x += player.speed * static_cast<game::coordinate>(horizontal) * delta_time;
    }

    static inline screen_coordinate to_screen_x(game::coordinate x) {
        auto result = static_cast<std::int_fast16_t>(x) % screen_width;
        if (result < 0)
            result += screen_width;
        return static_cast<screen_coordinate>(result);
    }

    static inline screen_coordinate to_screen_y(game::coordinate y) {
        auto result = static_cast<std::int_fast16_t>(y) % screen_height;
        if (result < 0)
            result += screen_height;
        return static_cast<screen_coordinate>(result);
    }

    // fill buffer in this function
    void draw() {
        // clear buffer (set all pixels to black)
        memset(buffer, 0, sizeof(buffer));
        // draw player at position with color
        const auto &player = game::current_player;
        const auto x = to_screen_x(player.position.x);
        const auto y = to_screen_y(player.position.y);
        buffer[y][x] = player.color;
    }

    // free game data in this function
    void finalize() {
    }
}

namespace game {
    player current_player{{engine::screen_width / 2.f, engine::screen_height / 2.f},
                          100,
                          {0, 0, 255, 255}};
}
