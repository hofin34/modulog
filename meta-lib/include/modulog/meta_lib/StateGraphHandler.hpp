#pragma once

#ifdef BRINGAUTO_TESTS
    #include <state_smurf/diagram/StateDiagram.hpp>
    #include <state_smurf/transition/StateTransition.hpp>
#endif

namespace modulog::meta_lib{
    class StateGraphHandler {
    public:
        #ifdef BRINGAUTO_TESTS
            static state_smurf::diagram::StateDiagram createStateDiagram();
        #endif

    };
}

