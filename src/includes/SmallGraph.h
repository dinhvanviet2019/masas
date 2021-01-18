/**
 * Name: Small Graph Header
 * Purpose: store graph contains: vertex, edges, weight of vertices
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * Limitation: only support for MTX
 * */
#ifndef SMALLGRAPH_H
#define SMALLGRAPH_H
#include "Graph.h"
class SmallGraph : public Graph {
public:
    SmallGraph(char * file);
    ~SmallGraph();    
protected:    
    void loadData();    
};
#endif