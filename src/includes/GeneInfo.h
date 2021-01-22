#ifndef GENEINFO_H
#define GENEINFO_H
#include "DSset.h"
#include "Owner.h"

class GeneInfo {
public:
    GeneInfo(Graph * graph) {
        n = graph->getGraphSize();
        dsSet = new DSSet(graph);
        owner = new Owner(graph);
    }

    bool contains(int v) {
        return dsSet->set[v];
    }

    bool isCovered(int v) {
        return owner->nOwner[v] > 0;
    }

    void addMainVertex(int u) {
        printf("Geneinfo: add vertex %d\n", u);
        if (u < 0 || u >= n || dsSet->set[u]) {
            printf("Geneinfo: add vertex %d completely\n", u);
            return;
        }
        dsSet->addMainVertex(u);
        owner->addMainVertex(u);
        printf("Geneinfo: add vertex %d completely\n", u);
    }

    void removeMainVertex(int u) {
        printf("Geneinfo: remove vertex %d\n", u);
        if (u < 0 || u >= n || !dsSet->set[u]) {
            printf("Geneinfo error: remove vertex %d completely\n", u);
            return;
        }
        dsSet->removeMainVertex(u);
        owner->removeMainVertex(u);
        printf("Geneinfo: remove vertex %d completely\n", u);
    }

    void clearInfo() {
        dsSet->clearInfo();
        owner->clearInfo();
    }
     
    void copy(GeneInfo* geneInfo) {
        dsSet->copy(geneInfo->getDSSet());
        owner->copy(geneInfo->getOwner());
    }

    DSSet* getDSSet() {
        return dsSet;
    }

    Owner* getOwner() {
        return owner;
    }

    void printInfo() {
        dsSet->printInfo();
        owner->printInfo();
    }

private:
    int n;
    Owner* owner;
    DSSet* dsSet;
    
};
#endif