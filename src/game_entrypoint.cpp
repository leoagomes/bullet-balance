#include <raylib.h>
#include <raylib-cpp.hpp>

#include "hotload/interface.hpp"

bb::State* bb_hotload_initialize(void) {
    return nullptr;
}

void bb_hotload_update(void) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        raylib::DrawText("second window!", 100, 100, 20, BLACK);
    EndDrawing();
}

void bb_hotload_release(bb::State* state) {}

