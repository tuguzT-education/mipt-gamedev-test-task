#pragma once

#include "Engine.h"

#include <cstddef>

namespace game {
    using entity = std::size_t;

    using coordinate = float;
    using speed = float;
    using engine::color;

    struct position final {
        coordinate x;
        coordinate y;
    };

    struct rectangle final {
        coordinate height;
        coordinate width;
    };

    struct circle final {
        coordinate radius;
    };

    /// FOR TEST ONLY

    struct player final {
        position position;
        speed speed;
        color color;
    };

    extern player current_player;
}
