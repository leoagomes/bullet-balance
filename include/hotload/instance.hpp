// Wraps hot reloading when built with that flag.
// All hot reloading logic should live here.
#pragma once

#include <functional>

#include "state.hpp"
#include "hotload/interface.hpp"

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

namespace bb::hotload {

class Instance {
private:
    std::function<State*(void)> _initialize;
    std::function<void(void)> _update;
    std::function<void(State*)> _release;

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
            _initialize = lib->get_function<GameState*(void)>("bb_hotload_initialize");
            _update = lib->get_function<void(void)>("bb_hotload_release");
            _release = lib->get_function<void(GameState*)>("bb_hotload_update");
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
        _initialize = ::bb_hotload_initialize;
        _update = ::bb_hotload_update;
        _release = ::bb_hotload_release;
    }
#endif

public:
    State* state = nullptr;

    Instance() {}
    ~Instance() {
        terminate();
    }

    State* initialize() {
        // this will either bind the dynamic library or just set
        // the static functions
        prepare();
        state = _initialize();
        return state;
    }

    void terminate() {
        if (state == nullptr) return;
        _release(state);
        state = nullptr;
    }

    void update() {
        #if defined(HOTLOAD)
        check_hot_reload();
        #endif

        _update();
    }
};

};

