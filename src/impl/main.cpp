#include <cstdio>
#include <cstring>
#include "GraphFactory.h"
#include "Graph.h"
#include "SmallGraph.h"
#include "MassiveGraph.h"
#include "GeneInfo.h"

int main() {
    printf("I Loves You, so you\n");
    const int MAX_LENS = 1024;
    char filePath[MAX_LENS];
    strcpy(filePath, "/home/ubuntu/Databases/mwds/dimacs/brock200-1.mtx");
    //SmallGraph *graph = new SmallGraph(filePath);
    Graph* graph = GraphFactory::createSmallGraph(filePath);
    GeneInfo* genInfo = new GeneInfo(graph);
    DSSet * set = genInfo->getDSSet();
    Owner* owner = genInfo->getOwner();
    //Owner* owner = new Owner(graph);
    genInfo->addMainVertex(2);    
    genInfo->addMainVertex(4);
    genInfo->addMainVertex(8);
    genInfo->addMainVertex(16);
    
    for (int i = 0; i < graph->getGraphSize(); i++) {
        printf("%d ", set->set[i]);
    }
    printf("\n");
    GeneInfo* genInfoClone = new GeneInfo(graph);
    genInfoClone->copy(genInfo);
    DSSet * setClone = genInfoClone->getDSSet();
    Owner* ownerClone = genInfoClone->getOwner();
    for (int i = 0; i < graph->getGraphSize(); i++) {
        printf("%d ", setClone->set[i]);
    }
    printf("\n");
    delete graph;    
    return 0;
}