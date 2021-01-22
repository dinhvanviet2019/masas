#ifndef DSSET_H
#define DSSET_H
#include "Graph.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

class DSSet{
public:
    DSSet(Graph * G);
    ~DSSet();
    double getValue();
    bool isDominated();
    void addMainVertex(int u);
    void removeMainVertex(int u);
    void copy(DSSet * dsset);
    void printInfo();
private:
    void clearInfo();
    Graph * _G;
    int n;
    bool * set;
    double value;    
};
#endif