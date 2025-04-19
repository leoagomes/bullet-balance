#pragma once

#include <cstdint>
#include <vector>
#include <tuple>

#include <entt/entt.hpp>

#include "types.hpp"

namespace bb {

// enum class entity_type : uint8_t {
//     none = 0,
//     bullet,
// };
//
// struct Entity {
// public:
//     id_t id = 0;
//     entity_type type = entity_type::none;
//     Vector2 position = Vector2::Zero();
//     float radius = 0;
//     Color color = Color::Black();
// };
//
// struct Bullet : public Entity {};

namespace components {

struct position {
    Vector2 value;
};
struct velocity {
    Vector2 value;
};
struct radius {
    float value;
};

};

struct State {
    uint32_t tick = 0;
    entt::registry registry;
};

};

