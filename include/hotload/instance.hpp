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

#if defined(PLATFORM_WEB)
#error "hotload not supported on web builds"
#endif
#endif

namespace bb::hotload {

class Instance {
private:
    std::function<State*(void)> _initialize;
    std::function<void(void)> _update;
    std::function<void(State*)> _release;

    std::function<void(State*)> _enter;
    std::function<void(void)> _leave;

#if defined(HOTLOAD)
    std::unique_ptr<dylib> lib = nullptr;

    // releases the loaded library and loads it again
    void reload_library() {
        TraceLog(LOG_INFO, "[hotload] releasing previous");
        if (lib) lib.reset();
        TraceLog(LOG_INFO, "[hotload] loading");
        lib = std::make_unique<dylib>(HOTLOAD_LIBRARY_DIR, HOTLOAD_LIBRARY);
    }

    // rebinds the library's functions
    void rebind_library() {
        TraceLog(LOG_INFO, "[hotload] binding");
        if (lib) {
            _initialize = lib->get_function<State*(void)>("bb_hotload_initialize");
            _update = lib->get_function<void(void)>("bb_hotload_update");
            _release = lib->get_function<void(State*)>("bb_hotload_release");

            _enter = lib->get_function<void(State*)>("bb_hotload_enter");
            _leave = lib->get_function<void(void)>("bb_hotload_leave");
        }
    }

    // reloads, rebinds
    void reload() {
        TraceLog(LOG_INFO, "[hotload] reloading");
        leave();
        reload_library();
        rebind_library();
        enter();
    }

    // just to maintain parity with the non-hotloaded version
    void prepare() {
        reload_library();
        rebind_library();
    }

    // does a hot reload check for the library
    void check_hot_reload() {
        if (IsKeyPressed(KEY_R)) {
            reload();
        }
    }

    void leave() {
        TraceLog(LOG_INFO, "[hotload] leaving previous");
        _leave();
    }

    void enter() {
        TraceLog(LOG_INFO, "[hotload] entering");
        _enter(state);
    }
#else
    void prepare() {
        _initialize = ::bb_hotload_initialize;
        _update = ::bb_hotload_update;
        _release = ::bb_hotload_release;

        _enter = ::bb_hotload_enter;
        _leave = ::bb_hotload_leave;
    }
#endif

public:
    State* state = nullptr;

    Instance() {}
    ~Instance() {
        if (state != nullptr) {
            TraceLog(LOG_WARNING, "[hotload] instance terminating without releasing the state");
        }
    }

    State* initialize() {
        TraceLog(LOG_DEBUG, "[hotload] initializing");

        // this will either bind the dynamic library or just set static functions.
        // when hotloading is enabled, it calls `_enter`.
        // IMPORTANT: this happens before the state is set up.
        prepare();

        // no state yet, call enter with null
        _enter(nullptr);

        // initializes the state
        state = _initialize();
        return state;
    }

    void terminate() {
        TraceLog(LOG_DEBUG, "[hotload] terminating");

        if (state == nullptr) return;

        _leave();
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

