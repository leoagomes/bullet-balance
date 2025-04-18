#include <raylib.h>
#include <raylib-cpp.hpp>

#include "game_hotload.hpp"

GameState* init_gamestate(void) {
    return nullptr;
}

void update_draw_frame(void) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        raylib::DrawText("second window!", 100, 100, 20, BLACK);
    EndDrawing();
}

void free_gamestate(GameState* state) {}

