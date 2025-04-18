#include <memory>

#include <raylib.h>
#include <raylib-cpp.hpp>

#include "game.hpp"

#include "hotload/interface.hpp"

std::unique_ptr<bb::Game> game_instance = nullptr;

bb::State* bb_hotload_initialize(void) {
    // create the initial version of the state.
    // called only once, on startup.
    auto state = new bb::State();
    game_instance->state = state;
    return state;
}

void bb_hotload_update(void) {
    // one update of the loop; do game logic.
    game_instance->update();
}

void bb_hotload_release(bb::State* state) {
    // free all resources, especially associated with the state itself.
    delete state;
}

void bb_hotload_enter(bb::State* state) {
    // entering this version of the code.
    // it is possible this is the _first_ load, in which case state is null
    game_instance = std::make_unique<bb::Game>(state);
}

void bb_hotload_leave(void) {
    // leaving this version of the code, release code-specific memory.
    // will be called before `release`.
    game_instance.reset();
}

