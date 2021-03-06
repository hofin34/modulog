#include <modulog/meta_lib/StateGraphHandler.hpp>

namespace modulog::meta_lib{
    #ifdef BRINGAUTO_TESTS
    state_smurf::diagram::StateDiagram StateGraphHandler::createStateDiagram() {
        state_smurf::diagram::StateDiagram stateDiagram;
        /// Creating Vertexes, they are accessible by name.
        auto start= stateDiagram.addVertex("Start");
        auto serverCreated = stateDiagram.addVertex("ServerCreated");
        auto creatingAgent = stateDiagram.addVertex("CreatingAgent");
        auto waitForMessage = stateDiagram.addVertex("WaitForMessage");
        auto processMessage = stateDiagram.addVertex("ProcessMessage");
        auto exiting = stateDiagram.addVertex("Exiting");
        auto stopAgent = stateDiagram.addVertex("StopAgent");
        auto cleanExit = stateDiagram.addVertex("CleanExit");

        stateDiagram.setStartVertex(start);

        stateDiagram.setEdge(start, serverCreated);
        stateDiagram.setEdge(serverCreated, creatingAgent);
        stateDiagram.setEdge(creatingAgent, creatingAgent);
        stateDiagram.setEdge(creatingAgent, waitForMessage);
        stateDiagram.setEdge(waitForMessage, processMessage);
        stateDiagram.setEdge(processMessage, waitForMessage);

        // Exit:
        stateDiagram.setEdge(exiting, stopAgent);
        stateDiagram.setEdge(exiting, cleanExit);
        stateDiagram.setEdge(stopAgent, stopAgent);
        stateDiagram.setEdge(stopAgent, cleanExit);

        // Interrupt:
        stateDiagram.setEdge(start, exiting);
        stateDiagram.setEdge(serverCreated, exiting);
        stateDiagram.setEdge(creatingAgent, exiting);
        stateDiagram.setEdge(waitForMessage, exiting);
        stateDiagram.setEdge(processMessage, exiting);



        return stateDiagram;
    }
    #endif
}

