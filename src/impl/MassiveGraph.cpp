/**
 * Name: MassiveGraph Implementation
 * Public Funcs: getADJListSize, getADJPnt, getWeight, loadData
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * */
#include "MassiveGraph.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

MassiveGraph::MassiveGraph(char * file):Graph(file) {
    loadData();
}

void MassiveGraph::loadData() {
    // Need to be done
}
