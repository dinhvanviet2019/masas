#ifndef DSSET_H
#define DSSET_H
#include "Graph.h"
#include <cstring>

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
    Graph * _G;
    int n;
    bool * set;
    double value;    
};
#endif