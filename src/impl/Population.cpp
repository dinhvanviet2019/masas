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
#include <time>

Population::Population(Graph* graph, int nGens) {
    this->graph = graph;
    this->nGens = nGens;    
    pops = new Gene*[nGens];    
    for (int i = 0; i < nGens; i++) {
        pops[i] = new Gene(graph);
    }
    tmpInfo = new TmpInfo(graph);
    bestKnown = new Gene(graph);
    CSlb = new Gene(graph);
}

Population::~Population() {
    for (int i = 0; i < graph->getGraphSize(); i++) {
        delete pops[i];
    }
    delete[] pops;
    delete bestKnown;
    delete CSlb;
}

void Population::initPopulation() {    
    for (int i = 0; i < nGens; i++) {
        pops[i]->setTmpInfo(tmpInfo);
        pops[i]->setCLSB(CSlb);
        pops[i]->construct();
        pops[i]->C_LS();
    }
    pops[0]->copyTo(bestKnown);
    for (int i = 1; i < nGens; i++) {
        if (bestKnown->getValue() > pops[i]->getValue()) {
            pops[i]->copyTo(bestKnown);
        }
    }
}

void Population::pdo(int i, int j) {
    srand(time(NULL));
    child->clearInfo();
    for (int i = 0; i < graph->getGraphSize(); i++) {
        if (pops[i]->contains(i) && pops[i]->contains(j) && rand() % 100 < beta1) {
            child->addMainVertex(i);
        } else {
            if (pops[i]->construct(i) || pops[i]->contains(j)) {
                if (!child->isCovered(i)) {
                    if (rand() % 100 < (100 - beta1)) {
                        child->addMainVertex(i);
                    }
                }
            } else {
                if (rand() % 100 < beta2) {
                    int* adjPtn = graph->getADJPnt(i);
                    for (int j = 0; j < graph->getADJListSize(i); j++) {
                        if (child.contains(*adjPtn)) {
                            child->removeMainVertex(*adjPtn);
                        }
                        adjPtn++;
                    }
                    child->addMainVertex(i);
                }
            }
        }
    }

}

void Population::poolUpdate(Gene* nextGen) {

}

void Population::run() {
    
}

// a communication