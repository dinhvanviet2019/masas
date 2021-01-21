/**
 * Name: Population Implemtation
 * Purpose: a  population
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * Limitation: only support for MTX
 * */
#include "Population.h"
#include <cstdlib>
#include <ctime>

Population::Population(Graph* graph) {
    this->graph = graph;    
    pops = new Gene*[nGens];    
    for (int i = 0; i < nGens; i++) {
        pops[i] = new Gene(graph);
    }
    tmpInfo = new TmpInfo(graph);
    bestKnown = new Gene(graph);
    child = new Gene(graph);
    CSlb = new Gene(graph);
}

Population::~Population() {
    for (int i = 0; i < graph->getGraphSize(); i++) {
        delete pops[i];
    }
    delete[] pops;
    delete tmpInfo;
    delete bestKnown;
    delete child;
    delete CSlb;
}

void Population::initPopulation() {  
    bestKnown->construct(false, nullptr);
    for (int i = 0; i < nGens; i++) {        
        pops[i]->construct(false, bestKnown);
        pops[i]->C_LS(tmpInfo, CSlb, bestKnown);
        if (pops[i]->getValue() < bestKnown->getValue()) {
            bestKnown->copy(pops[i]);
        }
    }
}

void Population::pdo(Gene* x, Gene* y, Gene* child) {
    initPopulation();
    srand(time(NULL));
    child->clearInfo();
    GeneInfo* genInfoX = x->getGenInfo();
    GeneInfo* genInfoY = y->getGenInfo();
    GeneInfo* genInfo = child->getGenInfo();
    for (int i = 0; i < graph->getGraphSize(); i++) {
        if (genInfoX->contains(i) && genInfoY->contains(i) && rand() % 100 < beta1) {
            genInfo->addMainVertex(i);
        } else {
            if (genInfoX->contains(i) || genInfoY->contains(i)) {
                if (!genInfo->isCovered(i)) {
                    if (rand() % 100 < (100 - beta1)) {
                        genInfo->addMainVertex(i);
                    }
                }
            } else {
                if (rand() % 100 < beta2) {
                    int* adjPtn = graph->getADJPnt(i);
                    for (int j = 0; j < graph->getADJListSize(i); j++) {
                        if (genInfo->contains(*adjPtn)) {
                            genInfo->removeMainVertex(*adjPtn);
                        }
                        adjPtn++;
                    }
                    genInfo->addMainVertex(i);
                }
            }
        }
    }
    child->construct(true, bestKnown);
}

void Population::poolUpdate(Gene* nextGen) {
    int worstID = 0;
    for (int i = 1; i < nGens; i++) {
        if (pops[worstID]->getValue() > pops[i]->getValue()) {
            worstID = i;
        }
    }
    if (pops[worstID]->getValue() > nextGen->getValue()) {
        pops[worstID]->copy(nextGen);
    } else {
        if (rand() % 1000 < 500) {
            pops[worstID]->copy(nextGen);
        }
    }
}

void Population::run() {
    int i = rand() % nGens;
    int j = rand() % nGens;
    Gene* x = pops[i];
    Gene* y = pops[j];
    pdo(x, y, child);
    int iter = 0;
    while (iter < maxIters) {
        child->C_LS(tmpInfo, CSlb, bestKnown);
        if (CSlb->getValue() < bestKnown->getValue()) {
            bestKnown->copy(CSlb);
        }
        poolUpdate(CSlb);
        if (CSlb->getValue() >= child->getValue()) {            
            i = rand() % nGens;
            j = rand() % nGens;
            x = pops[i];
            y = pops[j];
            pdo(x, y, child);
        } else {            
            pdo(x, CSlb, child);
        }
        iter++;           
    }     
}