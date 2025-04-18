#include <raylib.h>
#include <raylib-cpp.hpp>

#include "hotload/interface.hpp"

bb::State* bb_hotload_initialize(void) {
    // create the initial version of the state.
    // called only once, on startup.
    return nullptr;
}

void bb_hotload_update(void) {
    // one update of the loop; do game logic.
    BeginDrawing();
        ClearBackground(RAYWHITE);
        raylib::DrawText("second window!", 100, 100, 20, BLACK);
    EndDrawing();
}

void bb_hotload_release(bb::State* state) {
    // free all resources, especially associated with the state itself.
}

void bb_hotload_enter(bb::State* state) {
    // entering this version of the code.
    // it is possible this is the _first_ load, in which case state is null
    if (state == nullptr) return;
}

void bb_hotload_leave(void) {
    // leaving this version of the code, release code-specific memory.
    // will be called before `release`.
}

