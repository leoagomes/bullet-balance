#include <raylib-cpp.hpp>
#include <raylib.h>

#include "hotload/instance.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int screen_width = 800;
int screen_height = 450;
int target_fps = 60;

bb::hotload::Instance game;

int main(void) {
    // loads the game interface (hot-reloaded or static)
    game.initialize();

    // initialize the window
    raylib::Window window(screen_width, screen_height, "test window");

    // loop
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    window.SetTargetFPS(target_fps);
    while (!window.ShouldClose()) {
        game.update();
    }
#endif

    // cleans up the data
    game.terminate();
    return 0;
}

