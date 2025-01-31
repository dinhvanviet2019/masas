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
#include "GeneInfo.h"
#include "TmpInfo.h"

class Gene {
public:
    Gene(Graph * graph);
    ~Gene();
    void construct(bool inMating, Gene* bestKnown);
    void C_LS(TmpInfo* tmpInfo, Gene* CSlb, Gene* bestKnown);
    void copy(Gene* cell);    
    GeneInfo* getGenInfo();    
    double getValue();
    bool isCellLevel();
    bool contains(int v);
    void clearInfo();
    void printInfo();

private:
    const double alpha = 0.8;
    const int noimpro_iter = 1000;
    // graph info
    int n;
    Graph * G;
    // ds info
    GeneInfo* geneInfo;    
};
#endif