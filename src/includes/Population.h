/**
 * Name: Population Header
 * Purpose: a  population
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * Limitation: only support for MTX
 * */
#ifndef POPULATION_H
#define POPULATION_H

#include "Graph.h"
#include "Gene.h"
#include "TmpInfo.h"

class Population {
public:
    Population(Graph* graph);
    ~Population();
    void initPopulation();    
    void pdo(Gene* x, Gene* y);
    void poolUpdate(Gene * nextGen);
    void run();
    Gene* getGene(int i);    
private:
    const int nGens = 12;
    const int beta1 = 80;
    const int beta2 = 5;

    Graph* graph;
    Gene ** pops;
    TmpInfo * tmpInfo;
    Gene* bestKnown;
    Gene* CSlb;
    Gene* child;  
    void init();
};
#endif