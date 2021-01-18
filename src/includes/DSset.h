#ifndef DSSET_H
#define DSSET_H
#include "Graph.h"
typedef struct DSSet{
    DSSet(Graph * G) {
        _G = G;
        set = new bool[_G->getGraphSize()];
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
        set[u] = true;
        value -= _G->getWeight(u);
    }
    void setData(DSSet * dsset) {
        memcpy(this->set, dsset->set, _G->getGraphSize() * sizeof(bool));
        value = this->value;
    }
    Graph * _G;
    bool * set;
    double value;    
};
#endif