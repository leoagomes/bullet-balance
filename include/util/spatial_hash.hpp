#pragma once

#include <vector>
#include <cmath>

#include "types.hpp"

namespace bb {

class SpatialHash {
public:
    float spacing;                  /// the width/height of each cell
    size_t size;                    /// the total size of the hash table
    std::vector<size_t> cell_start; /// the cell array, each pointing to its entry list
    std::vector<id_t> cell_entries; /// the items in each cell
    std::vector<id_t> query_ids;    /// holds a query response

    SpatialHash(float spacing, size_t max_objects) :
        spacing(spacing),
        size(max_objects * 2),
        cell_start(size + 1),
        cell_entries(max_objects),
        query_ids(max_objects) {}
    ~SpatialHash() {}

    size_t hash_coordinates(int x, int y) {
        size_t hash = (x * 92837111) ^ (y * 689287499);
        return hash % size;
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

