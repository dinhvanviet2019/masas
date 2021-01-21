#include "MASAS.h"
#include "GraphFactory.h"
#include <cstdlib>
#include <ctime>

MASAS::MASAS() {
    // init MASAS object
}

MASAS::~MASAS() {
    // delete MASAS object
}

void MASAS::solve(char* filepath) {     
    Graph* graph = GraphFactory::createSmallGraph(filepath);
    Population * pops = new Population(graph);
    pops->run();
    pops->getBestKnownGene()->getGenInfo()->printInfo();
    delete pops;
    delete graph;
}

