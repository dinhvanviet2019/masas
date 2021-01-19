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
    pops->initPopulation();
    pops->run();
    delete pops;
    delete graph;
}

void MASAS::run() {

}

