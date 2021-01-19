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

    void addMainVertex(int u) {
        if (u < 0 || u >= n || dsSet->set[u]) {
            return;
        }
        dsSet->addMainVertex(u);
        owner->addMainVertex(u);
    }

    void removeMainVertex(int u) {
        if (u < 0 || u >= n || !dsSet->set[u]) {
            return;
        }
        dsSet->removeMainVertex(u);
        owner->removeMainVertex(u);
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

private:
    int n;
    Owner* owner;
    DSSet* dsSet;
    
};
#endif