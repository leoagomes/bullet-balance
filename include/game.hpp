#pragma once

#include "state.hpp"

namespace bb::game {

extern State* state;

State* create_state();
void free_state(State* state);
void update();

};

