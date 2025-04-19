#pragma once

#include <cstdint>

#include <entt/entt.hpp>

#include "types.hpp"
#include "util/spatial_hash.hpp"

namespace bb {

struct State {
    uint32_t tick = 0;
    entt::registry registry;
    SpatialHash spatial_hash;
};

};

