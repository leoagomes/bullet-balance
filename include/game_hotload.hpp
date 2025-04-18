// The main hot reload library interface.
// In a static build (HOTLOAD not defined) this is statically linked.
#pragma once

#include "shared.hpp"

#if defined(HOTLOAD) && (defined(_WIN32) || defined(_WIN64))
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

extern "C" {
LIB_EXPORT GameState* init_gamestate(void);
LIB_EXPORT void update_draw_frame(void);
LIB_EXPORT void free_gamestate(GameState*);
};
