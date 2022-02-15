#pragma once

#include "state_smurf/diagram/StateDiagram.hpp"
#include "state_smurf/transition/StateTransition.hpp"

namespace modulog::meta_lib{
    class StateGraphHandler {
    public:
        static state_smurf::diagram::StateDiagram createStateDiagram();

    };
}

