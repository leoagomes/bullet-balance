// The main hot reload library interface.
// In a static build (HOTLOAD not defined) this is statically linked.
#pragma once

#include "game_state.hpp"

#if defined(HOTLOAD) && (defined(_WIN32) || defined(_WIN64))
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

extern "C" {
LIB_EXPORT bb::GameState* bb_hotload_initialize(void);
LIB_EXPORT void bb_hotload_update(void);
LIB_EXPORT void bb_hotload_release(bb::GameState*);
};
