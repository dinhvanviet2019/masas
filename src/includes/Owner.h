#ifndef OWNER_H
#define OWNER_H
#include "Graph.h"

typedef struct Owner {
    Owner(Graph* G) {
        _G = G;
        n = G->getGraphSize();
        nCover = 0;
        nOwner = new int[_G->getGraphSize()];
        uVers = new int[_G->getGraphSize()];
        pos = new int[_G->getGraphSize()];
        numUVs = n;
        init();
    }

    ~Owner() {
        _G = nullptr;
        delete[] nOwner;
        delete[] uVers;
        delete[] pos;
    }

    void addMainVertex(int u) {       
        int* adjPnt = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            if (nOwner[*adjPnt] == 0) {
                nCover ++;
                removeFromUV(*adjPnt);
            }
            nOwner[*adjPnt]++;            
            adjPnt++;
        }
    }

    void removeMainVertex(int u) {
        int* adjPnt = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            if (nOwner[*adjPnt] == 0) {
                nCover --;
                addToUV(*adjPnt);
            }
            nOwner[*adjPnt]--;
            adjPnt++;
        }
    }

    // copy its owner to other
    void copy(Owner * owner) {
        nCover = owner->nCover;
        memcpy(nOwner, owner->nOwner,  n * sizeof(int));
        numUVs = owner->numUVs;
        memcpy(uVers, owner->uVers, n * sizeof(int));
    }

    void init() {
        // Owners
        nCover = 0;
        for (int i = 0; i < n; i++) {
            nOwner[i] = 0;
        }
        // reset uVers        
        numUVs = n;
        for (int i = 0; i < n; i++) {
            uVers[i] = i;
            pos[i] = i;
        }
    }

    void addToUV(int u) {
        if(pos[u] < 0) {
            pos[u] = numUVs;
            uVers[numUVs] = u;
            numUVs++;            
        }
    }

    void removeFromUV(int u) {
        if (pos[u] >= 0) {
            int posID = pos[u];
            int lastInstance = uVers[numUVs - 1];
            uVers[posID] = lastInstance;
            pos[lastInstance] = posID;
            pos[u] = -1;
            numUVs--;
        }
    }

    int calTO(int u) {
        int* adjPtn = _G->getADJPnt(u);
        int res = 0;
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            if (nOwner[*adjPtn] == 0) {
                res ++;
            }
            adjPtn++;
        }
        return res;
    }

    int nCover;
    int* nOwner;
    int* uVers;
    int numUVs;
    int* pos;
    Graph * _G;
    int n;
};
#endif