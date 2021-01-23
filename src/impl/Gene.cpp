#include "Gene.h"
#include "Random.h"
#include <cstdio>
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

GeneInfo* Gene::getGenInfo() {
    return geneInfo;
}

double Gene::getValue() {
    return geneInfo->getDSSet()->getValue();
}

bool Gene::isCellLevel() {
    //printf("");
    return (geneInfo->getOwner()->getNCover() == n);
}

bool Gene::contains(int v) {
    return geneInfo->getOwner()->getNOwner(v) > 0;
}


void Gene::copy(Gene* cell) {
    geneInfo->copy(cell->getGenInfo());
}

void Gene::clearInfo() {
        geneInfo->clearInfo();
}

void Gene::printInfo() {
    geneInfo->printInfo();
}

// change to GRASP with initialize
void Gene::construct(bool inMating, Gene* bestKnown) {
    int pmin, pmax;
    double * p;
    int nL;
    int * L;
    int nRCL;
    int * RCL;
    p = new double[n]; 
    RCL = new int[n]; 
    L = new int[n];  
    // initialize    
    DSSet* set = geneInfo->getDSSet();
    Owner* owner = geneInfo->getOwner();
    nL = 0;
    for (int i = 0; i < n; i++) {
        if (owner->getNOwner(i) == 0) {
            L[nL] = i;
            nL++;
        }
    }
    
    // GRASP
    while (owner->getNCover() < n && nL != 0) {    
        for (int i = 0; i < nL; i++) {
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
        #if DEBUG1
            printf("nCover = %d\n", owner->getNCover());
            printf("nL = %d\n", nL);
        #endif
        pmax = L[0];
        for (int i = 0; i < nL; i++) {
            if (p[pmax] < p[L[i]]) {
                pmax = L[i];
            }
        }
        double threshhold = p[pmin] + alpha * (p[pmax] - p[pmin]); // calculate threshold for RCL
        // create RCL        
        nRCL = 0;
        for (int i = 0; i < nL; i++) {
            if (p[L[i]] <= threshhold) {
                RCL[nRCL] = L[i];
                nRCL++;
            }
        }
        #if DEBUG1
            printf("nRCL = %d\n", nRCL);
        #endif
        // choose vertex
        int randID = Random::getRandInt(nRCL);
        int u = RCL[randID];
        //printf("chosen vertex = %d\n", u);
        if (inMating) {
            if (set->getValue() + G->getWeight(u) >= bestKnown->getValue()) {
                #if INFO
                    printf("INFO: warning - Good Luck for next time!\n");
                #endif
                return;
            }
        }
        geneInfo->addMainVertex(u);
        // update L
        // remove candidate which are dependent or (it and its neighbors are covered)
        for (int i = nL - 1; i >= 0; i--) {
            if (owner->getNOwner(L[i]) > 0 || owner->calTO(L[i]) == 0) {
                L[i] = L[nL - 1];
                nL --;
            }
        }
        //printf("nL = %d\n", nL);
    }
    delete[] L;
    delete[] RCL;
    delete[] p;
}

void Gene::C_LS(TmpInfo* tmpInfo, Gene* CSlb, Gene* bestKnown) {
    // init
    CSlb->copy(this);
    tmpInfo->init(geneInfo);
    int iter = 0;
    DSSet* set = geneInfo->getDSSet();
    Owner* owner = geneInfo->getOwner();
    while (iter < noimpro_iter) {
        #if INFO
            printf("iter = %d\n", iter);
            printf("Current Value = %0.2f and Current NCover = %d\n", set->getValue(), owner->getNCover());
            printf("CSlb value = %0.2f\n", CSlb->getValue());
        #endif
        if (owner->getNCover() == n) {
            if (set->getValue() < CSlb->getValue()) {
                CSlb->copy(this);
                #if INFO
                    printf("CSlb value = %0.2f\n", CSlb->getValue());
                #endif
                iter = 0;
            }                        
            int v = tmpInfo->findNextVertexToRemove(set);
            geneInfo->removeMainVertex(v);
            tmpInfo->updateConfWhenRemoveVertex(v);
            tmpInfo->create_N2_SCUpdatingList(v, owner);
            tmpInfo->updateSCFromList(geneInfo);
            continue;
        }
        int v = tmpInfo->findNextVertexToRemove(set);
        geneInfo->removeMainVertex(v);
        tmpInfo->updateConfWhenRemoveVertex(v);
        tmpInfo->create_N2_SCUpdatingList(v, owner);
        tmpInfo->updateSCFromList(geneInfo);
        tmpInfo->clearTabu();
        //v : = a vertex in CS with the highest value sc ( v ) and v ∈
        // tabu_list, breaking ties in the oldest one;
        while (owner->getNCover() < n) {
            int maxc = tmpInfo->findNextVertexToADD(geneInfo);            
            if (maxc == -1) {
                //no way to escape
                break;
            }
            
            if (set->getValue() + G->getWeight(maxc) >= bestKnown->getValue()) {
                break;
            }
            
            geneInfo->addMainVertex(maxc);
            tmpInfo->updateConfWhenAddMainVertex(maxc);
            tmpInfo->addToTabuList(maxc);
            tmpInfo->updatePP(owner);
            tmpInfo->create_SCUpatingtList_From_UVs(owner);
            tmpInfo->updateSCFromList(geneInfo);
        }
        iter ++;
    }
}