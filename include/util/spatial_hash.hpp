#pragma once

#include <vector>
#include <cmath>

#include "types.hpp"

namespace bb {

class SpatialHash {
public:
    float spacing;
    size_t table_size;
    std::vector<id_t> cell_start;
    std::vector<id_t> cell_entries;
    std::vector<id_t> query_ids;

    SpatialHash(float spacing, size_t max_objects) :
        spacing(spacing),
        table_size(max_objects * 2),
        cell_start(table_size + 1),
        cell_entries(max_objects),
        query_ids(max_objects) {}
    ~SpatialHash() {}

    size_t hash_coordinates(int x, int y) {
        size_t hash = (x * 92837111) ^ (y * 689287499);
        return hash % table_size;
    }

    size_t int_coordinate(int coord) {
        return std::floor(coord / spacing);
    }

    size_t hash_position(Vector2 position) {
        return hash_coordinates(
            int_coordinate(position.x),
            int_coordinate(position.y)
        );
    }

    void create(Vector2) {
    }
};

};

