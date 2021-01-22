 #include "GeneInfo.h"
 #include <cstdio>
 #include <cstring>

 GeneInfo::GeneInfo(Graph * graph) {
    n = graph->getGraphSize();
    dsSet = new DSSet(graph);
    owner = new Owner(graph);
}

DSSet* GeneInfo::getDSSet() {
    return dsSet;
}

Owner* GeneInfo::getOwner() {
    return owner;
}

bool GeneInfo::contains(int v) {
    return dsSet->isDominated(v);
}

bool GeneInfo::isCovered(int v) {
    return (owner->getNOwner(v) > 0);
}

void GeneInfo::addMainVertex(int u) {
    #if INFO
        printf("Geneinfo: add vertex %d\n", u);
    #endif
    if (u < 0 || u >= n || dsSet->isDominated(u)) {
        #if INFO
            printf("[Warning] Geneinfo: vertex %u is invalid range or in main set already\n", u);
        #endif
        return;
    }
    dsSet->addMainVertex(u);
    owner->addMainVertex(u);
    #if INFO
        printf("Geneinfo: add vertex %d completely\n", u);
    #endif
}

void GeneInfo::removeMainVertex(int u) {
    #if INFO
        printf("Geneinfo: remove vertex %d\n", u);
    #endif
    if (u < 0 || u >= n || !dsSet->isDominated(u)) {
        #if INFO
            printf("[Warning] Geneinfo: vertex %u is invalid range or not in main set\n", u);
        #endif
        return;
    }
    dsSet->removeMainVertex(u);
    owner->removeMainVertex(u);
    #if INFO
        printf("Geneinfo: remove vertex %d completely\n", u);
    #endif
}

void GeneInfo::clearInfo() {
    #if INFO
        printf("Clear Gene Info\n");
    #endif
    dsSet->clearInfo();
    owner->clearInfo();
}
    
void GeneInfo::copy(GeneInfo* geneInfo) {
    dsSet->copy(geneInfo->getDSSet());
    owner->copy(geneInfo->getOwner());
}

void GeneInfo::printInfo() {
    dsSet->printInfo();
    owner->printInfo();
}