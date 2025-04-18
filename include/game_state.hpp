#pragma once

#include <cstdint>
#include <vector>
#include <tuple>
#include <raylib-cpp.hpp>

#include "types.hpp"

namespace bb {

enum class entity_type : uint8_t {
    none = 0,
    bullet,
};

struct Entity {
public:
    id_t id = 0;
    entity_type type = entity_type::none;
    Vector2 position = Vector2::Zero();
    float radius = 0;
};

struct Bullet : public Entity {};

struct GameState {
    std::vector<Entity> entities;
    std::vector<std::tuple<id_t, size_t>> entity_map;
};

};

