#include <cstdio>
#include <cstring>
#include "GraphFactory.h"
#include "Graph.h"
#include "SmallGraph.h"
#include "MassiveGraph.h"
#include "GeneInfo.h"
#include "TmpInfo.h"

int main() {
    printf("I Loves You, so you\n");
    const int MAX_LENS = 1024;
    char filePath[MAX_LENS];
    strcpy(filePath, "/home/ubuntu/Databases/mwds/dimacs/brock200-1.mtx");
    //SmallGraph *graph = new SmallGraph(filePath);
    Graph* graph = GraphFactory::createSmallGraph(filePath);
    GeneInfo* genInfo = new GeneInfo(graph);
    TmpInfo* tmpInfo = new TmpInfo(graph);
    DSSet * set = genInfo->getDSSet();
    Owner* owner = genInfo->getOwner();    
    //Owner* owner = new Owner(graph);
    tmpInfo->init();
    tmpInfo->initSC(genInfo);
    genInfo->addMainVertex(2);    
    //genInfo->addMainVertex(4);
    //genInfo->addMainVertex(8);
    //genInfo->addMainVertex(16);
    owner->printUV();
    delete genInfo;
    delete tmpInfo;
    delete graph;
    return 0;
}