#include <cstdio>
#include <cstring>
#include "GraphFactory.h"
#include "Graph.h"
#include "SmallGraph.h"
#include "MassiveGraph.h"
#include "GeneInfo.h"
#include "TmpInfo.h"
#include "Gene.h"

int main() {
    printf("I Loves You, so you\n");
    const int MAX_LENS = 1024;
    char filePath[MAX_LENS];
    strcpy(filePath, "/home/ubuntu/Databases/mwds/dimacs/brock200-2.mtx");
    Graph* graph = GraphFactory::createSmallGraph(filePath);
    Gene* gene = new Gene(graph);
    GeneInfo* genInfo = gene->getGenInfo();
    TmpInfo* tmpInfo = new TmpInfo(graph);
    DSSet * set = genInfo->getDSSet();
    Owner* owner = genInfo->getOwner();
    gene->construct(false, NULL);    
    Gene* CSlb = new Gene(graph);
    Gene* bestKnown = new Gene(graph);
    CSlb->copy(gene);    
    bestKnown->copy(gene);  
    CSlb->C_LS(tmpInfo, CSlb, bestKnown);
    delete gene;
    delete CSlb;
    delete bestKnown;
    delete tmpInfo;
    delete graph;
    return 0;
}