/**
 * Name: Graph Factory Header
 * Public Funcs:
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 */
#ifndef GRAPHFACTORY_H
#define GRAPHFACTORY_H

#include "Graph.h"

class GraphFactory {
public:
    static Graph* createSmallGraph(char * file);
    static Graph* createMassiveGraph(char * file);
};

#endif