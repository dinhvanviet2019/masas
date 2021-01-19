#ifndef TMPINFO_H
#define TMPINFO_H
#include "Graph.h"
#include "GeneInfo.h"

typedef struct TmpInfo {
    TmpInfo(Graph* G) {
        _G = G;
        int n = _G->getGraphSize();
        tabulist = new int[n];
        inTabuList = new bool[n];
        conf = new bool[n];
        pp = new int[n];
        L = new int[n];
        sc = new double[n];
        nTabu = 0;
        scLen = 0;
        scUpdatingList = new int[_G->getGraphSize()];
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
    }

    void init() {
        int n = _G->getGraphSize();
        memset(tabulist, 0, n * sizeof(int));
        memset(inTabuList, 0, n * sizeof(bool));
        memset(conf, 1, n * sizeof(bool));
        memset(sc, 0, n * sizeof(double));
        memset(pp, 1, n * sizeof(int));
    }

    void addToTabuList(int u) {
        tabulist[nTabu] = u;
        nTabu++;
        inTabuList[u] = true;
    }
    
    void clearTabu() {
        for (int i = 0; i < nTabu; i++) {
            inTabuList[tabulist[i]] = false;
        }
        nTabu = 0;
    }

    void updateConfWhenAddMainVertex(int u) {                
        int* adjPtn = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            conf[*adjPtn] = true;
        }
    }

    void updateConfWhenRemoveVertex(int u) {
        conf[u] = false;
    }

    void initSC(GeneInfo* geneInfo) {
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        n = _G->getGraphSize();
        int sumTmp;
        for (int u = 0; u < n; u++) {
            if (!set->set[u]) {
                sc[u] = -1e9;
            } else {
                int * adjUPtn = _G->getADJPnt(u);
                sumTmp = 0;
                for (int i = 0; i < _G->getADJListSize(u); i++) {
                    if (owner->nOwner[*adjUPtn] == 1) {
                        sumTmp += pp[*adjUPtn];
                    }
                }
                sc[u] = -1 / _G->getWeight(u) * sumTmp;
            }
        }
    }
    
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
    }

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

    void reCalculateSC(GeneInfo* geneInfo) {
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        for (int i = 0; i < _G->getGraphSize(); i++) {
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

    void AddToL(int u) {
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
                    L[nL] = u;
                    nL ++;
                } else {
                    if (sc[L[nL]] < sc[u]) {                            
                        L[0] = u;
                        nL = 1;
                    } else {
                        if (sc[L[nL]] == sc[u]) {
                            L[nL] = u;
                            nL ++;
                        }
                    }
                }
            }
        }
        int v = L[rand() % nL];
        return v;
    }

    int findNextVertexToADD(GeneInfo* geneInfo){
        DSSet* set = geneInfo->getDSSet();
        Owner* owner = geneInfo->getOwner();
        clearL();
        for (int u = 0; u < n; u++) {
            if (!set->set[u] && conf[u] && owner->nOwner[u] == 0) {
                if (nL == 0) {                        
                    L[nL] = u;
                    nL ++;
                } else {
                    if (sc[L[nL]] < sc[u]) {                            
                        L[0] = u;
                        nL = 1;
                    } else {
                        if (sc[L[nL]] == sc[u]) {
                            L[nL] = u;
                            nL ++;
                        }
                    }
                }
            }
        }
        int v = L[rand() % nL];
        return v;
    }

    void updatePP(Owner* owner) {
        for (int i = 0; i < owner->numUVs; i++) {
            pp[owner->uVers[i]]++;
        }
    }

    void resetSCUpdatingList() {
        scLen = 0;
    }

    void addToSCUpdatingList(int u) {
        scUpdatingList[scLen] = u;
        scLen++;
    }

    void create_N2_SCUpdatingList(int u) {
        resetSCUpdatingList();
        int* adjPtn = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            int* adjUPtn = _G->getADJPnt(*adjPtn);
            for (int j = 0; j < _G->getADJListSize(*adjUPtn); j++) {
                addToSCUpdatingList(*adjUPtn);
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
    int scLen;
    int n;
};
#endif