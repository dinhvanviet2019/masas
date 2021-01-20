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
        set[u] = true;
        value += _G->getWeight(u);
    }
    void removeMainVertex(int u) {
        set[u] = false;
        value -= _G->getWeight(u);
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