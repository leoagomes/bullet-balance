#include <raylib-cpp.hpp>
#include <raylib.hpp>

#include "game.hpp"
#include "state.hpp"

using namespace bb;

State* game::state = nullptr;

State* game::create_state() {
    if (state) return state;

    state = new State{
        .tick = 0,
        .registry = entt::registry(),
    };
    return state;
}

void game::update() {
    auto& registry = state->registry;

    // setup
    if (state->tick == 0) {
        for (auto i = 0; i < 100u; i++) {
            const auto entity = registry.create();
            registry.emplace<components::position>(entity, Vector2(i * 1.f, i * 2.f));
            registry.emplace<components::radius>(entity, 10.f);
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

    // draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        // raylib::DrawText("first window!", 100, 100, 20, BLACK);

        registry
            .view<const components::position, const components::radius>()
            .each([](const auto& pos, const auto& radius){
                DrawCircle(pos.value.x, pos.value.y, radius.value, GRAY);
            });
    EndDrawing();

    state->tick++;
}

void game::free_state(State* state) {
    delete state;
}

