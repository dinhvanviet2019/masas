#include <cstdio>
#include <cstring>
#include "GraphFactory.h"
#include "Graph.h"
#include "SmallGraph.h"
#include "MassiveGraph.h"
#include "GeneInfo.h"
#include "TmpInfo.h"
#include "Gene.h"
#include "Population.h"
#include "Random.h"

int main() {    
    printf("I Loves You, so you\n");
    printf("true = %d, false = %d\n", true, false);
    
    const int MAX_LENS = 1024;
    char filePath[MAX_LENS];
    strcpy(filePath, "/home/ubuntu/Databases/mwds/dimacs/brock200-2.mtx");
    Graph* graph = GraphFactory::createSmallGraph(filePath);
    Population* pops = new Population(graph);  
    pops->initPopulation();
    pops->printInfo();
    pops->printBestKnownGen();       
    delete pops;
    delete graph;
   return 0;
}