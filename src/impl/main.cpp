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
    const int MAX_LENS = 1024;
    char filePath[MAX_LENS];
    strcpy(filePath, "/home/ubuntu/Databases/mwds/dimacs/brock200-2.mtx");
    Graph* graph = GraphFactory::createSmallGraph(filePath);
    Population* pops = new Population(graph);
    Gene* x = new Gene(graph);
    Gene* y = new Gene(graph);
    Gene* child = new Gene(graph);
    TmpInfo* tmpInfo = new TmpInfo(graph);
    x->construct(false, nullptr);
    y->construct(false, nullptr);
    printf("start o mating\n");
    //pops->pdo(y, y, child);
    x->printInfo();
    y->printInfo();
    //child->printInfo();
    delete tmpInfo;
    delete x;
    delete y;
    delete child;
    delete pops;    
    return 0;
}