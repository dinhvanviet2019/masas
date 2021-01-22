#include "TmpInfo.h"
#include "Random.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

TmpInfo::TmpInfo(Graph* G) {
    _G = G;
    n = _G->getGraphSize();
    conf = new bool[n];
    pp = new int[n];    
    sc = new double[n];
    nTabu = 0;
    tabulist = new int[n];
    inTabuList = new bool[n];
    scLen = 0;
    scUpdatingList = new int[n];
    inSCUpdatingList = new bool[n];
    nL = 0;
    L = new int[n];
}

TmpInfo::~TmpInfo() {
    _G = nullptr;
    delete[] conf;
    delete[] pp;
    delete[] sc;
    delete[] tabulist;
    delete[] inTabuList;    
    delete[] scUpdatingList;
    delete[] inSCUpdatingList;
    delete[] L;
}

void TmpInfo::init(GeneInfo* geneInfo) {
    for (int i = 0; i < n; i++) {
        conf[i] = 1;
    }
    memset(sc, 0, n * sizeof(double));
    for (int i = 0; i < n; i++) {
        pp[i] = 1;
    }
    memset(tabulist, 0, n * sizeof(int));
    memset(inTabuList, 0, n * sizeof(bool));
    initSC(geneInfo);        
    clearL();
}

int TmpInfo::findNextVertexToADD(GeneInfo* geneInfo){
    #if INFO
        printf("TmpInfo: start to find next vertex to add\n");
    #endif
    DSSet* set = geneInfo->getDSSet();
    Owner* owner = geneInfo->getOwner();
    clearL();
    for (int u = 0; u < n; u++) {
        #if DEBUG1 || DEBUG2        
            printf("DEBUG2: potential candidates\n");
            if (owner->getNOwner(u) == 0) {
                printf("potential candidate: %d, conf[%d] = %d\n", u, u, conf[u]);
            }
        #endif
        if (owner->getNOwner(u) == 0 && conf[u] == 1) {
            if (nL == 0) {                        
                addToL(u);
            } else {
                if (sc[L[nL]] < sc[u]) {                            
                    clearL();
                    addToL(u);
                } else {
                    if (sc[L[nL]] == sc[u]) {
                        addToL(u);
                    }
                }
            }
        }
    }
    if (nL == 0) {
        return -1;
    }
    #if DEBUG1 || DEBUG2
        printf("nL = %d\n", nL);
    #endif
    int v = L[Random::getRandInt(nL)];
    #if INFO
        printf("TmpInfo: Next vertex to add: vertex[%d] with sc = %0.2f\n", v, sc[v]);
    #endif
    return v;
}

int TmpInfo::findNextVertexToRemove(DSSet* set) {
    #if INFO
        printf("TmpInfo: start to find next vertex to remove\n");
    #endif
    clearL();
    for (int u = 0; u < n; u++) {
        #if DEBUG1 || DEBUG2        
            printf("DEBUG2: potential candidates\n");
            if (owner->getNOwner(u) == 0) {
                printf("potential candidate: %d, conf[%d] = %d\n", u, u, conf[u]);
            }
        #endif
        if (set->isDominated(u) && !inTabuList[u]) {
            if (nL == 0) {                        
                addToL(u);
            } else {
                if (sc[L[nL]] < sc[u]) {                            
                    clearL();
                    addToL(u);
                } else {
                    if (sc[L[nL]] == sc[u]) {
                        addToL(u);
                    }
                }
            }
        }
    }
    #if DEBUG1 || DEBUG2
        printf("nL = %d\n", nL);
    #endif
    int v = L[Random::getRandInt(nL)];
    #if INFO
        printf("TmpInfo: Next vertex to remove: vertex[%d] with sc = %0.2f\n", v, sc[v]);
    #endif
    return v;
}

void TmpInfo::updateConfWhenAddMainVertex(int u) {                
    #if INFO
        printf("INFO: update configuration change after adding vertex %d\n", u);
    #endif
    int v, k;    
    int* adjPtn = _G->getADJPnt(u);
    for (int i = 0; i < _G->getADJListSize(u); i++) {
        v = *adjPtn;   
        int* adjUPtn = _G->getADJPnt(v);
        for (int j = 0; j < _G->getADJListSize(v); j++) {
            k = *adjUPtn;
            conf[v] = 1;
            adjUPtn++;
        }
        adjPtn++;
    }
}

void TmpInfo::updateConfWhenRemoveVertex(int u) {    
    #if INFO
        printf("INFO: update configuration change after removing vertex %d\n", u);
    #endif    
    int* adjPtn = _G->getADJPnt(u);
    int v, k;
    for (int i = 0; i < _G->getADJListSize(u); i++) {
        v = *adjPtn;                
        int* adjUPtn = _G->getADJPnt(v);
        for (int j = 0; j < _G->getADJListSize(v); j++) {
            k = *adjUPtn;
            conf[k] = 1;
            adjUPtn++;
        }        
        adjPtn++;
    }
    conf[u] = 0;
}

void TmpInfo::updatePP(Owner* owner) {
    for (int i = 0; i < owner->getUVSize(); i++) {
        pp[owner->getUncoveredVertex(i)]++;
    }
}

void TmpInfo::updateSCFromList(GeneInfo* geneInfo) {
    int u, v;
    DSSet* set = geneInfo->getDSSet();    
    Owner* owner = geneInfo->getOwner();
    for (int li = 0; li < scLen; li++) {
        u = scUpdatingList[li];
        if (set->isDominated(u)) {
            int* adjPtn = _G->getADJPnt(u);            
            int sumPP = 0;
            for (int i = 0; i < _G->getADJListSize(u); i++) {
                v = *adjPtn;
                if (owner->getNOwner(v) == 1) {
                    sumPP += pp[v];
                }                    
                adjPtn++;
            }
            sc[u] = -1/_G->getWeight(u) * sumPP;
        } else {
            if (owner->getNOwner(u) == 0) {
                int* adjPtn = _G->getADJPnt(u);
                int sumPP = 0;
                for (int j = 0; j < _G->getADJListSize(u); j++) {
                    v = *adjPtn;
                    if (owner->getNOwner(*adjPtn) == 0) {
                        sumPP += pp[*adjPtn];
                    }
                    adjPtn++;
                }
                sc[u] = 1/_G->getWeight(u) * sumPP;
            }
        }
    }
}

void TmpInfo::create_N2_SCUpdatingList(int u, Owner* owner) {
    resetSCUpdatingList();
    int v, k;
    int* adjPtn = _G->getADJPnt(u);
    for (int i = 0; i < _G->getADJListSize(u); i++) {
        v = *adjPtn;
        int* adjUPtn = _G->getADJPnt(v);
        for (int j = 0; j < _G->getADJListSize(v); j++) {
            k = *adjUPtn;
            if (owner->getNOwner(k) == 0) {
                addToSCUpdatingList(k);
            }
            adjUPtn++;
        }
        adjPtn++;
    }
}

void TmpInfo::create_SCUpatingtList_From_UVs(Owner* owner) {
    resetSCUpdatingList();
    for (int i = 0; i < owner->getUVSize(); i++) {            
        addToSCUpdatingList(owner->getUncoveredVertex(i));
    }
}

void TmpInfo::printInfo() {    
    printf("configuration change\n");
    for (int i = 0; i < n; i++) {
        printf("conf[%d] = %d ", i, conf[i]);
    }
    printf("\n");

    printf("Scoring values: \n");        
    for (int i = 0; i < n; i++) {
        printf("sc[%d] = %0.2f ", i, sc[i]);
    }
    printf("\n");

    printf("pp\n");
    for (int i = 0; i < n; i++) {
        printf("pp[%d] = %d ", i, pp[i]);
    }
    printf("\n");

    #if DEBUG1 || DEBUG2
        printf("Tabu List\n");
        for (int i = 0; i < n; i++) {
            printf("tabulist[%d] = %d ", i, tabulist[i]);
        }
        printf("\n");
                
        printf("sc updating list\n");
        for (int i = 0; i < scLen; i++) {
            printf("scUpdatingList[%d] = %d ", i, scUpdatingList[i]);
        }
        printf("\n");
    #endif
}

void TmpInfo::initSC(GeneInfo* geneInfo) {
    resetSCUpdatingList();
    DSSet* set = geneInfo->getDSSet();
    Owner* owner = geneInfo->getOwner();
    n = n;
    int sumTmp;
    for (int u = 0; u < n; u++) {
        if (!set->isDominated(u)) {
            if (owner->getNOwner(u) == 0) {
                int * adjUPtn = _G->getADJPnt(u);
                sumTmp = 0;
                for (int i = 0; i < _G->getADJListSize(u); i++) {
                    if (owner->getNOwner(*adjUPtn) == 0) {
                        sumTmp += pp[*adjUPtn];
                    }
                    adjUPtn++;
                }
                sc[u] = 1 / _G->getWeight(u) * sumTmp;
            }
        } else {
            int * adjUPtn = _G->getADJPnt(u);
            sumTmp = 0;
            for (int i = 0; i < _G->getADJListSize(u); i++) {
                if (owner->getNOwner(*adjUPtn) == 1) {
                    sumTmp += pp[*adjUPtn];
                }
                adjUPtn++;
            }
            sc[u] = -1 / _G->getWeight(u) * sumTmp;
        }
    }
}

void TmpInfo::clearTabu() {
    for (int i = 0; i < nTabu; i++) {
        inTabuList[tabulist[i]] = 0;
    }
    nTabu = 0;
}

void TmpInfo::addToTabuList(int u) {
    tabulist[nTabu] = u;
    nTabu++;
    inTabuList[u] = 1;
}

void TmpInfo::resetSCUpdatingList() {
    scLen = 0;
    memset(inSCUpdatingList, 0, n);
}

void TmpInfo::addToSCUpdatingList(int u) {
    if (u < 0 || u >= n || inSCUpdatingList[u]) {
        return;
    }
    scUpdatingList[scLen] = u;
    inSCUpdatingList[u] = 1;
    scLen++;
}

void TmpInfo::addToL(int u) {
    L[nL] = u;
    nL++;
}

void TmpInfo::clearL() {
    nL = 0;
}