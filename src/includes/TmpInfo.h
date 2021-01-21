#ifndef TMPINFO_H
#define TMPINFO_H
#include "Graph.h"
#include "GeneInfo.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

typedef struct TmpInfo {
    TmpInfo(Graph* G) {
        _G = G;
        n = _G->getGraphSize();
        tabulist = new int[n];
        inTabuList = new bool[n];
        conf = new bool[n];
        pp = new int[n];
        L = new int[n];
        sc = new double[n];
        nTabu = 0;
        scLen = 0;
        scUpdatingList = new int[n];
        inSCUpdatingList = new bool[n];
    }

    ~TmpInfo() {
        _G = nullptr;
        delete[] tabulist;
        delete[] inTabuList;
        delete[] conf;
        delete[] pp;
        delete[] L;
        delete[] sc;
        delete[] scUpdatingList;
        delete[] inSCUpdatingList;
    }

    void init(GeneInfo* geneInfo) {
        srand(time(NULL));
        memset(tabulist, 0, n * sizeof(int));
        memset(inTabuList, 0, n * sizeof(bool));
        for (int i = 0; i < n; i++) {
            conf[i] = 1;
        }
        memset(sc, 0, n * sizeof(double));
        for (int i = 0; i < n; i++) {
            sc[i] = 0;
        }
        for (int i = 0; i < n; i++) {
            pp[i] = 1;
        }
        resetSCUpdatingList();
        initSC(geneInfo);        
        clearL();
    }

    void printInfo() {
        printf("Scoring: \n");        
        for (int i = 0; i < n; i++) {
            printf("%0.2f ", sc[i]);
        }
        printf("\n");
        printf("Tabu List\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", tabulist[i]);
        }
        printf("\n");
        printf("conf\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", conf[i]);
        }        
        printf("\n");
        printf("pp\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", pp[i]);
        }
        printf("\n");        
        printf("sc updating list\n");
        for (int i = 0; i < scLen; i++) {
            printf("%d ", scUpdatingList[i]);
        }
        printf("\n");
    }
    
    void addToTabuList(int u) {
        tabulist[nTabu] = u;
        nTabu++;
        inTabuList[u] = 1;
    }
    
    void clearTabu() {
        for (int i = 0; i < nTabu; i++) {
            inTabuList[tabulist[i]] = 0;
        }
        nTabu = 0;
    }

    void updateConfWhenAddMainVertex(int u) {                
        int* adjPtn = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            //conf[*adjPtn] = 1;
            
            int* adjUPtn = _G->getADJPnt(*adjPtn);
            for (int j = 0; j < _G->getADJListSize(*adjUPtn); j++) {
                conf[*adjUPtn] = 1;
                adjUPtn++;
            }
            adjPtn++;
        }
    }

    void updateConfWhenRemoveVertex(int u) {        
        int* adjPtn = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            //conf[*adjPtn] = 1;
            
            int* adjUPtn = _G->getADJPnt(*adjPtn);
            for (int j = 0; j < _G->getADJListSize(*adjUPtn); j++) {
                conf[*adjUPtn] = 1;
                adjUPtn++;
            }
            
            adjPtn++;
        }
        conf[u] = 0;
    }

    void initSC(GeneInfo* geneInfo) {
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        n = n;
        int sumTmp;
        for (int u = 0; u < n; u++) {
            if (!set->set[u]) {
                if (owner->nOwner[u] == 0) {
                    int * adjUPtn = _G->getADJPnt(u);
                    sumTmp = 0;
                    for (int i = 0; i < _G->getADJListSize(u); i++) {
                        if (owner->nOwner[*adjUPtn] == 0) {
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
                    if (owner->nOwner[*adjUPtn] == 1) {
                        sumTmp += pp[*adjUPtn];
                    }
                    adjUPtn++;
                }
                sc[u] = -1 / _G->getWeight(u) * sumTmp;
            }
        }
    }
    /*
    void updateSC(int u, GeneInfo* geneInfo) {
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        int* adjPtn = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            int* adjUPtn = _G->getADJPnt(*adjPtn);            
            for (int j = 0; j < _G->getADJListSize(*adjUPtn); j++) {
                int v = *adjUPtn;
                int *adjVPtn = _G->getADJPnt(*adjUPtn);
                int tmpK = 0;
                for (int k = 0; k < _G->getADJListSize(*adjVPtn); k++) {
                    if (!set->set[v]) {
                        if (owner->nOwner[k] == 0) {
                            tmpK += pp[k];
                        }
                    } else {
                        if (owner->nOwner[k] == 1) {
                            tmpK += pp[k];
                        }
                    }                    
                    adjVPtn++;
                }
                if (set->set[v]) {
                    sc[v] = -1 / _G->getWeight(v) * tmpK;
                } else {
                    sc[v] = 1/ _G->getWeight(v) * tmpK;
                }
                adjUPtn++;
            }
            adjPtn++;
        }
    }*/

    void updateSCFromList(GeneInfo* geneInfo) {
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        for (int li = 0; li < scLen; li++) {
            int i = scUpdatingList[i];
        if (set->set[i]) {
            int* adjPtn = _G->getADJPnt(i);
            int tmpK = 0;
            for (int k = 0; k < _G->getADJListSize(i); k++) {
                if (owner->nOwner[*adjPtn] == 1) {
                    tmpK += pp[*adjPtn];
                }                    
                adjPtn++;
            }
            sc[i] = -1/_G->getWeight(i) * tmpK;
        } else {
            if (owner->nOwner[i] == 0) {
                int* adjPtn = _G->getADJPnt(i);
                int tmpK = 0;
                for (int k = 0; k < _G->getADJListSize(i); k++) {
                    if (owner->nOwner[*adjPtn] == 0) {
                        tmpK += pp[*adjPtn];
                    }                        
                }
                sc[i] = 1/_G->getWeight(i) * tmpK; 
            }
        }
        }
    }
/*
    void reCalculateSC(GeneInfo* geneInfo) {
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        for (int i = 0; i < n; i++) {
            if (set->set[i]) {
                int* adjPtn = _G->getADJPnt(i);
                int tmpK = 0;
                for (int k = 0; k < _G->getADJListSize(i); k++) {
                    if (owner->nOwner[*adjPtn] == 1) {
                        tmpK += pp[*adjPtn];
                    }                    
                    adjPtn++;
                }
                sc[i] = -1/_G->getWeight(i) * tmpK;
            } else {
                if (owner->nOwner[i] == 0) {
                    int* adjPtn = _G->getADJPnt(i);
                    int tmpK = 0;
                    for (int k = 0; k < _G->getADJListSize(i); k++) {
                        if (owner->nOwner[*adjPtn] == 0) {
                            tmpK += pp[*adjPtn];
                        }                        
                    }
                    sc[i] = 1/_G->getWeight(i) * tmpK; 
                }
            }
        }
    }
*/
    void addToL(int u) {
        L[nL] = u;
        nL++;
    }

    void clearL() {
        nL = 0;
    }

    int findNextVertexToRemove(DSSet* set) {
        clearL();
        for (int u = 0; u < n; u++) {
            if (set->set[u] && !inTabuList[u]) {
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
        int v = L[rand() % nL];
        printf("find next verte to remove: vertex[%d] with sc = %0.2f\n", v, sc[v]);
        return v;
    }

    int findNextVertexToADD(GeneInfo* geneInfo){
        printf("start to find next vertex to add\n");
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        clearL();
        //printInfo();
        for (int u = 0; u < n; u++) {
            if (owner->nOwner[u] == 0) {
                //printf("potential candidate: %d, conf[%d] = %d, 1 = %d \n", u, u, conf[u], true);
            }
            if (owner->nOwner[u] == 0 && conf[u] == 1) {
                //printf("potential candidate: %d\n", u);
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
        //geneInfo->getOwner()->printInfo();
        //printInfo();
        if (nL == 0) {
            return -1;
        }
        printf("nL = %d\n", nL);
        int v = L[rand() % nL];
        printf("find next vertex to add: vertex[%d] with sc = %0.2f\n", v, sc[v]);
        return v;
    }

    void updatePP(Owner* owner) {
        for (int i = 0; i < owner->numUVs; i++) {
            pp[owner->uVers[i]]++;
        }
    }

    void resetSCUpdatingList() {
        scLen = 0;
        memset(inSCUpdatingList, 0, n);
    }


    void addToSCUpdatingList(int u) {
        if (u < 0 || u >= n || inSCUpdatingList[u]) {
            return;
        }
        scUpdatingList[scLen] = u;
        inSCUpdatingList[u] = 1;
        scLen++;
    }

    void create_N2_SCUpdatingList(int u, Owner* owner) {
        resetSCUpdatingList();
        int* adjPtn = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            int* adjUPtn = _G->getADJPnt(*adjPtn);
            for (int j = 0; j < _G->getADJListSize(*adjUPtn); j++) {  
                if (owner->nOwner[*adjUPtn] == 0) {
                    addToSCUpdatingList(*adjUPtn);
                }
                adjUPtn++;
            }
            adjPtn++;
        }
    }

    void create_SCUpatingtList_From_UVs(Owner* owner) {
        resetSCUpdatingList();
        for (int i = 0; i < owner->numUVs; i++) {            
            addToSCUpdatingList(owner->uVers[i]);
        }
    }

    Graph* _G;
    int nTabu;
    int * tabulist;
    bool * inTabuList;
    bool * conf;
    int * pp;
    double * sc;
    int nL;
    int * L;
    int * scUpdatingList;
    bool *inSCUpdatingList;
    int scLen;
    int n;
};
#endif