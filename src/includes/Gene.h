/**
 * Name: Gene Header
 * Purpose: a single gene in population
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * Limitation: only support for MTX
 * */

#ifndef GENE_H
#define GENE_H
#include "Graph.h"
#include "DSset.h"
#include "Owner.h"
#include "TmpInfo.h"

class Gene {
public:
    Gene(Graph * graph);
    ~Gene();
    void construct(bool inMating, Gene* bestKnown);
    void C_LS();
    void copyTo(Gene* cell);
    void setData(DSSet* set, Owner* owner, TmpInfo* tmpInfo);
    void setTmpInfo(TmpInfo* tmpInfo);
    double getValue();
    void setCLSB(Gene* CLsb) {
        this->CSlb = CLsb;
    }
    bool contains(int v) {
        return owner->nOwner[v];
    }
    bool isCovered(int v) {
        // need to implement
    }
    void clearInfo() {
        // need to implement
    }
    void addMainVertex(int v) {
        // need to implement
    }
    void removeMainVertex(int v){
        // need to implement
    }
private:
    const double alpha = 0.8;
    const int noimpro_iter = 50000;
    // graph info
    int n;
    Graph * G;
    // ds info
    DSSet* set;
    // ownner info
    Owner* owner;    
};
#endif