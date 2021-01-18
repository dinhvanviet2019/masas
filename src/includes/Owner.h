#ifndef OWNER_H
#define OWNER_H
#include "Graph.h"

typedef struct Owner {
    Owner(Graph* G) {
        _G = G;
        nCover = 0;
        nOwner = new int[_G->getGraphSize()];
        uVers = new int[_G->getGraphSize()];
        pos = new int[_G->getGraphSize()];
        numUVs = 0;
    }

    ~Owner() {
        _G = nullptr;
        delete[] nOwner;
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

    void setData(Owner * owner) {
        nCover = owner->nCover;
        memcpy(nOwner, owner->nOwner, _G->getGraphSize());
        numUVs = owner->numUVs;
        memcpy(uVers, owner->uVers, _G->getGraphSize());
    }

    void resetUV() {
        numUVs = 0;
        for (int i = 0; i < _G->getGraphSize(); i++) {
            pos[i] = -1;
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
        if (pos[u] > 0) {
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
};
#endif