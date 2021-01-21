#ifndef DSSET_H
#define DSSET_H
#include "Graph.h"
#include <cstring>
#include <cstdio>

typedef struct DSSet{
    DSSet(Graph * G) {
        _G = G;
        n = _G->getGraphSize();
        set = new bool[n];
        memset(set, 0, n * sizeof(bool));
        value = 0;
    }
    ~DSSet() {
        _G = nullptr;
        delete[] set;
    }
    void addMainVertex(int u) {
        set[u] = 1;
        value += _G->getWeight(u);
    }
    void removeMainVertex(int u) {
        set[u] = 0;
        value -= _G->getWeight(u);
    }

    void clearInfo() {
        value = 0;
        memset(set, 0, n * sizeof(bool));
    }

    void copy(DSSet * dsset) {
        memcpy(set, dsset->set, n * sizeof(bool));
        value = dsset->value;
    }

    void printInfo() {
        printf("Value = %0.2f\n", value);
        printf("Set List: ");
        for (int i = 0; i < n; i++) {
            if (set[i]) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }
    Graph * _G;
    int n;
    bool * set;
    double value;    
};
#endif