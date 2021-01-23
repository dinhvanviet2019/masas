#ifndef TMPINFO_H
#define TMPINFO_H
#include "Graph.h"
#include "GeneInfo.h"

class TmpInfo {
public:    
    TmpInfo(Graph* G);
    ~TmpInfo();
    void init(GeneInfo* geneInfo);    
    int findNextVertexToRemove(DSSet* set);
    int findNextVertexToADD(GeneInfo* geneInfo);
    void updateConfWhenAddMainVertex(int u);
    void updateConfWhenRemoveVertex(int u);
    void create_N2_SCUpdatingList(int u, Owner* owner);
    void create_SCUpatingtList_From_UVs(Owner* owner);
    void updateSCFromList(GeneInfo* geneInfo);    
    void addToTabuList(int u);
    void clearTabu();
    void updatePP(Owner* owner);
    void printInfo();

private:
    void initSC(GeneInfo* geneInfo);
    void addToSCUpdatingList(int u);
    void resetSCUpdatingList();
    void addToL(int u);
    void clearL();    
    Graph* _G;
    int n;
    bool * conf;
    int * pp;
    double * sc;
    int nTabu;
    int * tabulist;
    bool * inTabuList;
    int nL;
    int * L;
    int scLen;
    int * scUpdatingList;
    bool *inSCUpdatingList;    
};
#endif