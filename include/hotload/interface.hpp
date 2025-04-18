// The main hot reload library interface.
// In a static build (HOTLOAD not defined) this is statically linked.
#pragma once

#include "state.hpp"

#if defined(HOTLOAD) && (defined(_WIN32) || defined(_WIN64))
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

extern "C" {
/// Initializes (allocates) the main state.
LIB_EXPORT bb::State* bb_hotload_initialize(void);
/// Updates the game loop.
LIB_EXPORT void bb_hotload_update(void);
/// Releases the state (very last thing before exiting).
LIB_EXPORT void bb_hotload_release(bb::State*);

/// Enter a new version of the library.
/// Called with `nullptr` _before_ `initialize` when the game is loaded.
LIB_EXPORT void bb_hotload_enter(bb::State*);
/// Leaves this version of the library.
/// Called before releasing the state.
LIB_EXPORT void bb_hotload_leave(void);
};
