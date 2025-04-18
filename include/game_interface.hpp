// Wraps hot reloading when built with that flag.
// All hot reloading logic should live here.
#pragma once

#include <functional>

#include "shared.hpp"
#include "game_hotload.hpp"

#if defined(HOTLOAD)
#include <memory>
#include <dylib.hpp>
#include <raylib.h>

#define HOTLOAD_LIBRARY_DIR "./"
// the hotload library name should be defined as HOTLOAD_LIBRARY
#ifndef HOTLOAD_LIBRARY
#error "HOTLOAD_LIBRARY must be defined"
#endif
#endif

class GameInterface {
private:
    std::function<GameState*(void)> _init_gamestate;
    std::function<void(GameState*)> _free_gamestate;
    std::function<void(void)> _update_draw_frame;

#if defined(HOTLOAD)
    std::unique_ptr<dylib> lib = nullptr;

    // releases the loaded library and loads it again
    void reload_library() {
        TraceLog(LOG_INFO, "[hotload] releasing old library");
        if (lib) lib.reset();
        TraceLog(LOG_INFO, "[hotload] loading library");
        lib = std::make_unique<dylib>(HOTLOAD_LIBRARY_DIR, HOTLOAD_LIBRARY);
    }

    // rebinds the library's functions
    void rebind_library() {
        TraceLog(LOG_INFO, "[hotload] binding functions");
        if (lib) {
            _init_gamestate = lib->get_function<GameState*(void)>("init_gamestate");
            _free_gamestate = lib->get_function<void(GameState*)>("free_gamestate");
            _update_draw_frame = lib->get_function<void(void)>("update_draw_frame");
        }
    }

    // reloads, rebinds
    void reload() {
        TraceLog(LOG_INFO, "[hotload] reloading");
        reload_library();
        rebind_library();
    }

    // just to maintain parity with the non-hotloaded version
    void prepare() {
        reload();
    }

    // does a hot reload check for the library
    void check_hot_reload() {
        if (IsKeyPressed(KEY_R)) {
            reload();
        }
    }
#else
    void prepare() {
        _init_gamestate = ::init_gamestate;
        _free_gamestate = ::free_gamestate;
        _update_draw_frame = ::update_draw_frame;
    }
#endif

public:
    GameState* state = nullptr;

    GameInterface() {}
    ~GameInterface() {
        terminate();
    }

    GameState* initialize() {
        // this will either bind the dynamic library or just set
        // the static functions
        prepare();
        state = _init_gamestate();
        return state;
    }

    void terminate() {
        if (state == nullptr) return;
        _free_gamestate(state);
        state = nullptr;
    }

    void update() {
        #if defined(HOTLOAD)
        check_hot_reload();
        #endif

        _update_draw_frame();
    }
};

