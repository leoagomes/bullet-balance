#pragma once

#include "state.hpp"

namespace bb {

class Game {
public:
    State* state;
    Game(State* state) : state(state) {
        // at this point, state might be null
    }

    void update();
};

};

