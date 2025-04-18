#include <raylib-cpp.hpp>
#include <raylib.h>

#include "main.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(void);

int screen_width = 800;
int screen_height = 450;
int target_fps = 60;

int main(void) {
    raylib::Window window(screen_width, screen_height, "test window");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(target_fps);

    while (!window.ShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    return 0;
}

void UpdateDrawFrame(void) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        raylib::DrawText("first window!", 100, 100, 20, BLACK);
    EndDrawing();
}

