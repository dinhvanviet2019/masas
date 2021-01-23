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
    Gene* x = new Gene(graph);    
    Gene* y = new Gene(graph);
    Gene* tmpGen = new Gene(graph);
    Gene* CSlb = new Gene(graph);
    Gene* bestKnown = new Gene(graph);
    Gene* child = new Gene(graph);    
    TmpInfo* tmpInfo = new TmpInfo(graph);
    x->construct(false, nullptr);
    bestKnown->copy(x);
    x->printInfo();
    for (int i = 0; i < 1; i++) {
        printf("iter = %d\n",i);
        tmpGen->copy(x);
        tmpGen->C_LS(tmpInfo, CSlb, bestKnown);
        /*
        if (CSlb->isCellLevel() && bestKnown->getValue() > CSlb->getValue()) {
            CSlb->printInfo();
            bestKnown->copy(CSlb);            
        }
        */
    }
    
    delete x;
    
    delete tmpInfo;
    delete y;
    delete CSlb;
    delete bestKnown;
    delete child;
    
    delete pops;
   return 0;
}