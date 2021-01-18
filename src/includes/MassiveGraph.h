/**
 * Name: Small Graph Header
 * Purpose: store graph contains: vertex, edges, weight of vertices
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * Limitation: 
 * */
#ifndef MASSIVEGRAPH_H
#define MASSIVEGRAPH_H
#include "Graph.h"
class MassiveGraph : public Graph {
public:
    MassiveGraph(char * file);
    ~MassiveGraph();    
protected:    
    void loadData();    
};
#endif