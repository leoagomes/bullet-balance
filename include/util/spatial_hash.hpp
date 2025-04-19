#pragma once

#include <vector>
#include <cmath>

#include <entt/entt.hpp>

#include "components.hpp"

namespace bb {

class SpatialHash {
public:
    /// the width/height of each cell
    float spacing;
    /// the total size of the hash table
    size_t size;
    /// the cell array, each pointing to its entry list
    std::vector<size_t> cell_starts;
    /// the entities in each cell
    std::vector<entt::entity> cell_entities;
    /// holds a query response
    std::vector<entt::entity> query_ids;

    SpatialHash(float spacing, size_t max_objects) :
        spacing(spacing),
        size(max_objects * 2),
        cell_starts(size + 1, 0),
        cell_entities(max_objects, static_cast<entt::entity>(entt::null)),
        query_ids()
    {
        query_ids.reserve(max_objects);
    }
    ~SpatialHash() {}

    size_t hash_coordinates(int x, int y) {
        size_t hash = (x * 92837111) ^ (y * 689287499);
        return hash % size;
    }

    int int_coordinate(float coord) {
        return std::floor(coord / spacing);
    }

    size_t hash_position(Vector2 position) {
        return hash_coordinates(
            int_coordinate(position.x),
            int_coordinate(position.y)
        );
    }

    void create(entt::registry& registry) {
        auto view = registry.view<const components::position>();

        std::fill(cell_starts.begin(), cell_starts.end(), 0);
        std::fill(cell_entities.begin(), cell_entities.end(), entt::null);

        view.each([&](const auto& position){
            auto hash = hash_position(position.value);
            cell_starts[hash]++;
        });

        size_t start = 0;
        for (auto i = 0; i < size; i++) {
            start += cell_starts[i];
            cell_starts[i] = start;
        }
        cell_starts[size] = start;

        view.each([&](const auto entity, const auto& position){
            auto hash = hash_position(position.value);
            cell_starts[hash]--;
            cell_entities[cell_starts[hash]] = entity;
        });
    }

    std::vector<entt::entity>& query(Vector2 position, float distance) {
        int x0 = int_coordinate(position.x - distance);
        int y0 = int_coordinate(position.y - distance);
        int x1 = int_coordinate(position.x + distance);
        int y1 = int_coordinate(position.y + distance);

        query_ids.clear();

        for (auto x = x0; x <= x1; x++) {
            for (auto y = y0; y <= y1; y++) {
                auto hash = hash_coordinates(x, y);
                auto start = cell_starts[hash];
                auto end = cell_starts[hash + 1];

                for (auto i = start; i < end; i++) {
                    query_ids.push_back(cell_entities[i]);
                }
            }
        }

        return query_ids;
    }
};

};

