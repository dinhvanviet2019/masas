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
void Gene::construct(bool inMating, Gene* bestKnown) {
    srand(time(NULL));   
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
        if (owner->nOwner[i] == 0) {
            L[nL] = i;
            nL++;
        }
    }
    
    // GRASP
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
            if (p[pmax] < p[L[i]]) {
                pmax = L[i];
            }
        }
        double threshhold = p[pmin] + alpha * (p[pmax] - p[pmin]); // calculate threshold for RCL
        //printf("threshold %0.2f \n", threshhold);
        // create RCL
        nRCL = 0;
        for (int i = 0; i < nL; i++) {
            if (p[L[i]] <= threshhold) {
                RCL[nRCL] = L[i];
                nRCL++;
            }
        }
        //printf("nRCL = %d\n", nRCL);
        // choose vertex
        int randID = rand() % nRCL;
        int u = RCL[randID];
        //printf("chosen vertex = %d\n", u);
        if (inMating) {
            if (set->value  + G->getWeight(u) >= bestKnown->getValue()) {
                return;
            }
        }
        geneInfo->addMainVertex(u);
        // update L
        // remove candidate which are dependent or (it and its neighbors are covered)
        for (int i = nL - 1; i >= 0; i--) 
            if (owner->nOwner[L[i]] == 0 || owner->calTO(L[i]) == 0) {
                L[i] = L[nL - 1];
                nL --;
        }
        //printf("nL = %d\n", nL);
    }
    delete[] L;
    delete[] RCL;
    delete[] p;
}

void Gene::C_LS(TmpInfo* tmpInfo, Gene* CSlb, Gene* bestKnown) {
    srand(time(NULL));
    // init
    CSlb->copy(this);
    tmpInfo->init(geneInfo);
    tmpInfo->printInfo();   
    int iter = 0;
    DSSet* set = geneInfo->getDSSet();
    Owner* owner = geneInfo->getOwner();
    while (iter < noimpro_iter) {
        printf("iter = %d\n", iter);
        printf("start 1st stage\n");
        if (owner->nCover == n) {
            set->printInfo();
            if (set->value < CSlb->getValue()) {
                CSlb->copy(this);
                printf("CSlb value = %0.2f\n", CSlb->getValue());
                iter = 0;
            }                        
            int v = tmpInfo->findNextVertexToRemove(set);
            printf("1st removed vertex = %d\n", v);
            geneInfo->removeMainVertex(v);
            printf("start to update config completely\n");
            tmpInfo->updateConfWhenRemoveVertex(v);
            printf("update config completely\n");
            tmpInfo->create_N2_SCUpdatingList(v, owner);
            printf("create N2 SCUpdateing List completely\n");
            tmpInfo->updateSCFromList(geneInfo);
            printf("nCover = %d \n", owner->nCover);
            continue;
        }
        //printf("test point\n");
        printf("CSlb value = %0.2f\n", CSlb->getValue());
        int v = tmpInfo->findNextVertexToRemove(set);
        printf("2nd removed vertex = %d\n", v);
        geneInfo->removeMainVertex(v);
        printf("start to update config completely\n");
        tmpInfo->updateConfWhenRemoveVertex(v);
        printf("update config completely\n");
        tmpInfo->create_N2_SCUpdatingList(v, owner);
        printf("create N2 SCUpdateing List completely\n");
        tmpInfo->updateSCFromList(geneInfo);
        printf("update SC completely\n");
        tmpInfo->clearTabu();
        printf("clear tabu completely\n");
        //v : = a vertex in CS with the highest value sc ( v ) and v ∈
        // tabu_list, breaking ties in the oldest one;
        printf("complete 1st stage\n");
        printf("start 2nd stage!\n");
        while (owner->nCover < n) {
            printf("nCover = %d\n", owner->nCover);
            int maxc = tmpInfo->findNextVertexToADD(geneInfo);            
            printf("vertex maxc = %d\n", maxc);
            if (maxc == -1) {
                //no way to escape
                return;
            }
            if (set->value + G->getWeight(maxc) >= bestKnown->getValue()) {
                break;
            }
            geneInfo->addMainVertex(maxc);
            printf("add vertex completely\n");
            tmpInfo->updateConfWhenAddMainVertex(maxc);
            printf("update config when add completely\n");
            tmpInfo->addToTabuList(maxc);
            printf("add tabulist PP\n");
            tmpInfo->updatePP(owner);
            printf("update PP completely");
            tmpInfo->create_SCUpatingtList_From_UVs(owner);
            printf("create SC updating list from uvs\n");
            tmpInfo->updateSCFromList(geneInfo);
            printf("update sc completely\n");      
        }
        printf("complete 2nd stage!\n");
        iter ++;
    }
}