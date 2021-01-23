#ifndef OWNER_H
#define OWNER_H
#include "Graph.h"

class GeneInfo;

class Owner {
friend class GeneInfo;
public:    
    Owner(Graph* G);
    ~Owner();
    int getNCover();
    int getNOwner(int u);
    void addMainVertex(int u);
    void removeMainVertex(int u);            
    void copy(Owner * owner);
    int calTO(int u);
    int getUVSize();
    int getUncoveredVertex(int id);
    void printInfo();
private:
    void addToUV(int u);
    void removeFromUV(int u);
    void clearInfo();
    int testNCover();
    int nCover;
    int* nOwner;
    int* uVers;
    int numUVs;
    int* pos;
    Graph * _G;
    int n;
};
#endif