#include "Gene.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

Gene::Gene(Graph * graph) {
    G = graph;
    n = G->getGraphSize();
    geneInfo = new GeneInfo(graph);
}

Gene::~Gene() {
   delete geneInfo;
}

// change to GRASP with initialize
void Gene::construct(TmpInfo* tmpInfo, bool inMating, Gene* bestKnown) {
    srand(time(NULL));   
    int pmin, pmax;
    int * RCL;
    double * p;
    int * L;
    int nL;
    int nRCL;
    RCL = new int[n];
    L = new int[n];    
    // initialize    
    nRCL = 0;
    DSSet* set = geneInfo->getDSSet();
    Owner* owner = geneInfo->getOwner();
    for (int i = 0; i < n; i++) {
        if (owner->nOwner[i] == 0) {
            L[i] = i;
        }
    }     
    // GRASP
    nL = n;
    while (owner->nCover < n) {
        for (int i = 0; i < n; i++) {
            p[L[i]] = G->getWeight(L[i]) / owner->calTO(L[i]);
        } 
        // min p
        pmin = L[0];
        for (int i = 0; i < nL; i++) {
            if (p[pmin] > p[L[i]]) {
                pmin = L[i];
            }
        }
        // max p
        pmax = L[0];
        for (int i = 0; i < nL; i++) {
            if (p[pmax] > p[L[i]]) {
                pmax = L[i];
            }
        }
        double threshhold = pmin + alpha * (pmax - pmin); // calculate threshold for RCL
        // create RCL
        for (int i = 0; i < nL; i++) {
            if (p[L[i]] <= threshhold) {
                RCL[nRCL] = L[i];
                nRCL++;
            }
        }
        // choose vertex
        int randID = rand() % nRCL;
        int u = RCL[randID];
        if (set->value  + G->getWeight(u) >= bestKnown->getValue()) {
            return;
        }
        set->addMainVertex(u);
        owner->addMainVertex(u);                          
        // update L
        // remove candidate which are dependent or (it and its neighbors are covered)
        for (int i = nL - 1; i >= 0; i--) 
            if (owner->nOwner[L[i]] == 0 || owner->calTO(L[i]) == 0) {
                L[i] = L[nL - 1];
                nL --;
        }
    }
    delete[] L;
    delete[] RCL;
    delete[] p;
}

void Gene::C_LS(TmpInfo* tmpInfo, Gene* CSlb) {
    srand(time(NULL));
    // init
    CSlb->copy(this);
    tmpInfo->init();        
    tmpInfo->initSC(geneInfo);
    tmpInfo->clearL();
    int iter = 0;
    DSSet* set = geneInfo->getDSSet();
    Owner* owner = geneInfo->getOwner();
    while (iter < noimpro_iter) {
        if (owner->nCover == n) {
            if (set->value < CSlb->getValue()) {
                CSlb->copy(this);
                iter = 0;
            }                        
            int v = tmpInfo->findNextVertexToRemove(set);            
            set->removeMainVertex(v);
            owner->removeMainVertex(v);
            tmpInfo->updateConfWhenRemoveVertex(v);
            tmpInfo->create_N2_SCUpdatingList(v);
            tmpInfo->updateSCFromList(geneInfo);
            continue;
        }
        int v = tmpInfo->findNextVertexToRemove(set);
        set->removeMainVertex(v);
        owner->removeMainVertex(v);
        tmpInfo->updateConfWhenRemoveVertex(v);
        tmpInfo->create_N2_SCUpdatingList(v);
        tmpInfo->updateSCFromList(geneInfo);
        tmpInfo->clearTabu();
        //v : = a vertex in CS with the highest value sc ( v ) and v ∈
        // tabu_list, breaking ties in the oldest one;
        while (owner->nCover < n) {
            int maxc = tmpInfo->findNextVertexToADD(geneInfo);
            if (set->value + G->getWeight(maxc) >= CSlb->getValue()) {
                break;
            }
            set->addMainVertex(maxc);
            owner->addMainVertex(maxc);
            tmpInfo->updateConfWhenAddMainVertex(maxc);
            tmpInfo->updateSC(v, geneInfo);
            tmpInfo->addToTabuList(maxc);            
            tmpInfo->updatePP(owner);
            tmpInfo->create_SCUpatingtList_From_UVs(owner);
            tmpInfo->updateSCFromList(geneInfo);        
        }
        iter ++;
    }
}