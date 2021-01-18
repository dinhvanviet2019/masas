/**
 * Name: Graph Factory Implementation
 * Public Funcs: createSmallGraph, createSmallGraph
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * */
#include "GraphFactory.h"
#include "SmallGraph.h"
#include "MassiveGraph.h"

Graph* createSmallGraph(char * file) {
    return new SmallGraph(file);
}

Graph* createMassiveGraph(char * file) {
    return new MassiveGraph(file);
}