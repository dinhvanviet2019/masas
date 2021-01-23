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
#include "Random.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

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
    tmpGen = new Gene(graph);
}

Population::~Population() {    
    delete[] pops;
    delete tmpInfo;
    delete bestKnown;
    delete child;
    delete CSlb;
    delete tmpGen;
}

void Population::initPopulation() {  
    bestKnown->construct(false, nullptr);
    for (int i = 0; i < nGens; i++) {
        tmpGen->clearInfo();        
        tmpGen->construct(false, bestKnown);
        tmpGen->C_LS(tmpInfo, CSlb, bestKnown);
        pops[i]->copy(CSlb);        
        if (pops[i]->getValue() < bestKnown->getValue()) {
            bestKnown->copy(pops[i]);
        }
    }
}

void Population::pdo(Gene* x, Gene* y, Gene* child) {
    child->clearInfo();
    GeneInfo* genInfoX = x->getGenInfo();
    GeneInfo* genInfoY = y->getGenInfo();
    GeneInfo* genInfo = child->getGenInfo();
    for (int i = 0; i < graph->getGraphSize(); i++) {
        if (genInfoX->contains(i) && genInfoY->contains(i) && Random::getRandInt(100) < beta1) {
            genInfo->addMainVertex(i);
        } else {
            if (genInfoX->contains(i) || genInfoY->contains(i)) {
                if (!genInfo->isCovered(i)) {
                    if (Random::getRandInt(100) < (100 - beta1)) {
                        genInfo->addMainVertex(i);
                    }
                }
            } else {
                if (Random::getRandInt(100) < beta2) {
                    int* adjPtn = graph->getADJPnt(i);
                    for (int j = 0; j < graph->getADJListSize(i); j++) {
                        int v = *adjPtn;
                        if (genInfo->contains(v)) {
                            genInfo->removeMainVertex(v);
                        }
                        adjPtn++;
                    }
                    genInfo->addMainVertex(i);
                }
            }
        }
    }
    #if INFO
        printf("start repair gene\n");
    #endif
    //child->printInfo();
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
        if (Random::getUniform() <= 0.5 ) {
            pops[worstID]->copy(nextGen);
        }
    }
}

void Population::run() {
    initPopulation();
    int i = Random::getRandInt(nGens);
    int j = Random::getRandInt(nGens);
    Gene* x = pops[i];
    Gene* y = pops[j];
    pdo(x, y, child);
    int iter = 0;
    while (iter < maxIters) {
        double min_value_ofXY = fmin(x->getValue(), y->getValue());
        child->C_LS(tmpInfo, CSlb, bestKnown);
        if (CSlb->getValue() < bestKnown->getValue()) {
            bestKnown->copy(CSlb);
        }
        poolUpdate(CSlb);
        if (CSlb->getValue() >= min_value_ofXY) {
            i = Random::getRandInt(nGens);
            j = Random::getRandInt(nGens);
            x = pops[i];
            y = pops[j];
            pdo(x, y, child);
        } else {            
            pdo(x, CSlb, child);
        }
        iter++;           
    }     
}

Gene* Population::getBestKnownGene() {
    return bestKnown;
}

void Population::printInfo() {
    for (int i = 0; i < nGens; i++) {
        printf("Gen %d Info\n", i);
        pops[i]->printInfo();
    }
}

void Population::printBestKnownGen() {
    printf("Best Known Gen\n");
    bestKnown->printInfo();
}