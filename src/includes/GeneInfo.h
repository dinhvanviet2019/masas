#ifndef GENEINFO_H
#define GENEINFO_H
#include "DSSet.h"
#include "Owner.h"

class GeneInfo {
public:
    GeneInfo(Graph * graph);
    DSSet* getDSSet();
    Owner* getOwner();
    void addMainVertex(int u);
    void removeMainVertex(int u);
    void clearInfo();
    void copy(GeneInfo* geneInfo);
    bool contains(int v);
    bool isCovered(int v);    
    void printInfo();
private:
    int n;
    Owner* owner;
    DSSet* dsSet;    
};
#endif