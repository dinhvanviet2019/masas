#include "Gene.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

Gene::Gene(Graph * graph) {
    G = graph;
    n = G->getGraphSize();
    set = new DSSet(G);
    owner = new Owner(G);
    tmpInfo = new TmpInfo(G);
}

Gene::~Gene() {
   delete set;
   delete owner;
}

void Gene::copyTo(Gene* cell) {    
    cell->setData(set, owner, tmpInfo);    
}

void Gene::setData(DSSet* set, Owner * owner, TmpInfo* tmpInfo) {
    this->set->setData(set);
    this->owner->setData(owner);
    this->tmpInfo = tmpInfo;
}

void Gene::setTmpInfo(TmpInfo* tmpInfo) {
    this->tmpInfo = tmpInfo;
}

double Gene::getValue() {
    return set->value;
}

// change to GRASP with initialize
void Gene::construct() {
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
    for (int i = 0; i < n; i++) {
        L[i] = i;
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

void Gene::C_LS() {
    srand(time(NULL));
    // init
    CSlb->setData(set, owner);
    tmpInfo->init();        
    tmpInfo->initSC(set, owner);
    tmpInfo->clearL();
    int iter = 0;
    while (iter < noimpro_iter) {
        if (owner->nCover == n) {
            if (set->value < CSlb->getValue()) {
                CSlb->setData(set, owner);
                iter = 0;
            }                        
            int v = tmpInfo->findNextVertexToRemove(set);            
            set->removeMainVertex(v);
            owner->removeMainVertex(v);
            tmpInfo->updateConfWhenRemoveVertex(v);
            tmpInfo->create_N2_SCUpdatingList(v);
            tmpInfo->updateSCFromList(set, owner);
            continue;
        }
        int v = tmpInfo->findNextVertexToRemove(set);
        set->removeMainVertex(v);
        owner->removeMainVertex(v);
        tmpInfo->updateConfWhenRemoveVertex(v);
        tmpInfo->create_N2_SCUpdatingList(v);
        tmpInfo->updateSCFromList(set, owner);
        tmpInfo->clearTabu();
        //v : = a vertex in CS with the highest value sc ( v ) and v ∈
        // tabu_list, breaking ties in the oldest one;
        while (owner->nCover < n) {
            int maxc = tmpInfo->findNextVertexToADD(set, owner);
            if (set->value + G->getWeight(maxc) >= CSlb->getValue()) {
                break;
            }
            set->addMainVertex(maxc);
            owner->addMainVertex(maxc);
            tmpInfo->updateConfWhenAddMainVertex(maxc);
            tmpInfo->updateSC(v, set, owner);
            tmpInfo->addToTabuList(maxc);            
            tmpInfo->updatePP(owner);
            tmpInfo->create_SCUpatingtList_From_UVs(owner);
            tmpInfo->updateSCFromList(set, owner);        
        }
        iter ++;
    }
}