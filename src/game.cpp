#include <raylib-cpp.hpp>
#include <raylib.hpp>

#include "game.hpp"

void bb::Game::update() {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        raylib::DrawText("third window!", 100, 100, 20, BLACK);
    EndDrawing();
}
