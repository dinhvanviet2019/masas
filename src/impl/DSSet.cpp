 #include "DSSet.h"
 #include <cstdio>

 DSSet::DSSet(Graph * G) {
    _G = G;
    n = _G->getGraphSize();
    set = new bool[n];
    clearInfo();
}

DSSet::~DSSet() {
    _G = nullptr;
    delete[] set;
}

double DSSet::getValue() {
    return value;
}

bool DSSet::isDominated(int u) {
    return set[u];
}

void DSSet::addMainVertex(int u) {
    set[u] = 1;
    value += _G->getWeight(u);
}

void DSSet::removeMainVertex(int u) {
    set[u] = 0;
    value -= _G->getWeight(u);
}

void DSSet::clearInfo() {
    value = 0;
    memset(set, 0, n * sizeof(bool));
}

void DSSet::copy(DSSet * dsset) {
    memcpy(set, dsset->set, n * sizeof(bool));
    value = dsset->value;
}

void DSSet::printInfo() {
    printf("Value = %0.2f\n", value);
    printf("Set List: ");
    for (int i = 0; i < n; i++) {
        if (set[i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");
    #if DEBUG2
    printf("[DEBUG LEVEL 2] Set Information:")
    for (int i = 0; i < n; i++) {        
        printf("%d ", set[i]);
    } 
    printf("\n");
    #endif
}