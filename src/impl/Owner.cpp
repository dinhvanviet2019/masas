#include "Owner.h"
#include <cstdio>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <cstring>

Owner::Owner(Graph* G) {
    _G = G;
    n = G->getGraphSize();
    nCover = 0;
    nOwner = new int[_G->getGraphSize()];
    numUVs = n;
    uVers = new int[_G->getGraphSize()];
    pos = new int[_G->getGraphSize()];
    clearInfo();
}

Owner::~Owner() {
    _G = nullptr;
    delete[] nOwner;
    delete[] uVers;
    delete[] pos;
}

int Owner::getNCover() {
    return nCover;
}

int Owner::getNOwner(int u) {
    return nOwner[u];
}

void Owner::addMainVertex(int u) {
    int v;
    int* adjPnt = _G->getADJPnt(u);
    for (int i = 0; i < _G->getADJListSize(u); i++) {
        v = *adjPnt;
        if (nOwner[v] == 0) {
            nCover ++;
            removeFromUV(v);
        }
        nOwner[v]++;            
        adjPnt++;
    }
    #if DEBUG1
        printf("[DEBUG LEVEL 2] Add Vertex To Owner\n");
        printf("add vertex %d\n", u);
        printf("nCover = %d\nChecked nCover = %d\n", nCover, testNCover());    
    #endif
}

void Owner::removeMainVertex(int u) {
    int* adjPnt = _G->getADJPnt(u);
    for (int i = 0; i < _G->getADJListSize(u); i++) {
        if (nOwner[*adjPnt] == 1) {
            nCover --;
            addToUV(*adjPnt);
        }
        nOwner[*adjPnt]--;
        adjPnt++;
    }
    #if DEBUG2
        printf("[DEBUG LEVEL 2] Remove Vertex To Owner\n");
        printf("remove vertex %d\n", u);
        printf("nCover = %d\nChecked nCover = %d\n", nCover, testNCover());    
    #endif
}

// copy its owner to other
void Owner::copy(Owner * owner) {
    nCover = owner->nCover;
    memcpy(nOwner, owner->nOwner,  n * sizeof(int));
    numUVs = owner->numUVs;
    memcpy(uVers, owner->uVers, n * sizeof(int));
}

int Owner::getUVSize() {
    return numUVs;
}

int Owner::getUncoveredVertex(int id) {
    return uVers[id];
}

void Owner::addToUV(int u) {
    if(pos[u] < 0) {
        pos[u] = numUVs;
        uVers[numUVs] = u;
        numUVs++;            
    }
}

void Owner::removeFromUV(int u) {
    if (pos[u] >= 0) {
        int posID = pos[u];
        int lastInstance = uVers[numUVs - 1];
        uVers[posID] = lastInstance;
        pos[lastInstance] = posID;
        pos[u] = -1;
        numUVs--;
    }
}

void Owner::clearInfo() {
    nCover = 0;
    memset(nOwner, 0, n * sizeof(int));
    numUVs = 0;
    memset(uVers, 0, n * sizeof(int));
}

int Owner::testNCover() {
    int res = 0;
    for (int i = 0; i < n; i++) {
        res += (nOwner[i] > 0);
    }
    return res;
}

void Owner::printInfo() {
    printf("nCover = %d\n", nCover);
    printf("nOwner: ");
    for (int i = 0; i < n; i++) {
        printf("nOwner[%d] = %d, ", i, nOwner[i]);
    }
    printf("\n");
    #if DEBUG2
        printf("UV: ");
        for (int i = 0; i < numUVs; i++) {
            printf("UV[%d] = %d, ", i, uVers[i]);
        }
        printf("\n");
    #endif
}

int Owner::calTO(int u) {
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