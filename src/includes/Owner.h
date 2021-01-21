#ifndef OWNER_H
#define OWNER_H
#include "Graph.h"
#include <cstdio>
#include <stdlib.h>     /* exit, EXIT_FAILURE */

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
        bool checkmate =  nCover == testNCover();
    }

    void removeMainVertex(int u) {
        int* adjPnt = _G->getADJPnt(u);
        for (int i = 0; i < _G->getADJListSize(u); i++) {
            if (nOwner[*adjPnt] == 1) {
                nCover --;
                addToUV(*adjPnt);
            }
            nOwner[*adjPnt]--;
            adjPnt++;
        }
        bool checkmate =  nCover == testNCover();
    }

    int testNCover() {
        int res = 0;
        for (int i = 0; i < n; i++) {
            res += nOwner[i] > 0;
        }
        return res;
    }

    void clearInfo() {
        nCover = 0;
        memset(nOwner, 0, n * sizeof(int));
        numUVs = 0;
        memset(uVers, 0, n * sizeof(int));
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

    void printInfo() {
        printf("nCover = %d\n", nCover);
        printf("nOwner\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", nOwner[i]);
        }
        printf("\n");
        printf("UV\n");
        for (int i = 0; i < numUVs; i++) {
            printf("%d ", uVers[i]);
        }
        printf("\n");
    }

    void printUV() {
        for (int i = 0; i < numUVs; i++) {
            printf("%d ", uVers[i]);
        }
        printf("\n");
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