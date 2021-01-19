#ifndef GENEINFO_H
#define GENEINFO_H
#include "DSset.h"
#include "Owner.h"

class GeneInfo {
public:
    GeneInfo(Graph * graph) {
        n = graph->getGraphSize();
        owner = new Owner(graph);
        dsSet = new DSSet(graph);
    }

    void addMainVertex(int u) {
        if (u < 0 || u >= n || dsSet->set[u]) {
            return;
        }
        owner->addMainVertex(u);
        dsSet->addMainVertex(u);
    }

    void removeMainVertex(int u) {
        if (u < 0 || u >= n || !dsSet->set[u]) {
            return;
        }
        owner->removeMainVertex(u);
        dsSet->removeMainVertex(u);
    }

    void setData(DSSet * dsset, Owner * owner) {
        if (dsSet != nullptr) {
            this->dsSet->setData(dsSet);
        }
        if (owner != nullptr) {
            this->owner->setData(owner);
        }
    }

    Owner* getOwner() {
        return owner;
    }

    DSSet* getDSSet() {
        return dsSet;
    }

private:
    int n;
    Owner* owner;
    DSSet* dsSet;    
};
#endif