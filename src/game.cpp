#include <raylib-cpp.hpp>
#include <raylib.h>
#include <raylib.hpp>
#include <string>

#include "game.hpp"
#include "Functions.hpp"
#include "state.hpp"
#include "components.hpp"

using namespace bb;

#define MAX_SPATIAL_HASH_ENTITIES 10000
#define SPATIAL_HASH_SPACING 20.f

State* game::state = nullptr;

State* game::create_state() {
    if (state) return state;

    state = new State{
        .tick = 0,
        .registry = entt::registry(),
        .spatial_hash = SpatialHash(SPATIAL_HASH_SPACING, MAX_SPATIAL_HASH_ENTITIES),
    };
    return state;
}

void game::update() {
    auto& registry = state->registry;

    // setup
    if (state->tick == 0) {
        for (auto i = 0; i < 10u; i++) {
            const auto entity = registry.create();
            registry.emplace<components::position>(entity, Vector2(100 + i * 1.f, 100 + i * 2.f));
            registry.emplace<components::radius>(entity, 10.f);
            registry.emplace<components::draw_color>(entity, Color::Gray());
            if (i % 2 == 0) {
                registry.emplace<components::velocity>(entity, Vector2(i * 1.f, 0));
            }
        }
    }

    // update
    auto deltatime = GetFrameTime();
    registry
        .view<components::position, const components::velocity>()
        .each([=](auto& position, const auto& velocity) {
            position.value += velocity.value * deltatime;
        });

    auto& spatial_hash = state->spatial_hash;
    spatial_hash.create(registry);

    registry
        .view<const components::position, const components::radius, components::draw_color>()
        .each([&](auto entity, const auto& position, const auto& radius, auto& color){
            color.value = Color::Gray();

            auto& candidates = spatial_hash.query(position.value, 2 * radius.value);
            TraceLog(LOG_INFO, std::to_string(candidates.size()).c_str());
            for (auto candidate: candidates) {
                if (entity == candidate)
                    continue;

                auto [c_position, c_radius] = registry.get<const components::position, const components::radius>(candidate);
                auto distance_squared = position.value.DistanceSqr(c_position.value);
                auto range = (radius.value + c_radius.value);
                auto range_squared = range * range;
                if (distance_squared <= range_squared) {
                    color.value = Color::Yellow();
                    break;
                }
            }
        });

    // draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        // raylib::DrawText("first window!", 100, 100, 20, BLACK);

        registry
            .view<const components::position, const components::radius, const components::draw_color>()
            .each([](const auto& pos, const auto& radius, const auto& color){
                DrawCircle(pos.value.x, pos.value.y, radius.value, color.value);
            });

        raylib::DrawText(std::to_string(GetFPS()), 10, 20, 20, BLACK);
    EndDrawing();

    state->tick++;
}

void game::free_state(State* state) {
    delete state;
}

