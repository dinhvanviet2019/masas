#ifndef DSSET_H
#define DSSET_H
#include "Graph.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

class GeneInfo;

class DSSet{
friend class GeneInfo;
public:
    DSSet(Graph * G);
    ~DSSet();
    double getValue();
    bool isDominated(int u);
    void addMainVertex(int u);
    void removeMainVertex(int u);
    void clearInfo();
    void copy(DSSet * dsset);
    void printInfo();
private:
    Graph * _G;
    int n;
    bool * set;
    double value;    
};
#endif