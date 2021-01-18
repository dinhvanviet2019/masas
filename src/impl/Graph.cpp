/**
 * Name: Graph Implementation
 * Public Funcs: getADJListSize, getADJPnt, getWeight, loadData
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * */
#include "Graph.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

using namespace std;

Graph::Graph(char * file) {
    strcpy(filePath, file);
    n = 0;
    m = 0;
    adjVertexPtr = NULL;
    adjListSize = NULL;
    adjIndicator = NULL;
    weights = NULL;
}

Graph::~Graph() {
    if (!adjVertexPtr) {
        delete[] adjVertexPtr;
    }
    if (!adjListSize) {
        delete[] adjListSize;
    }
    if (!adjIndicator) {
        delete[] adjIndicator;
    }
    if (!weights) {
        delete[] weights;
    }
}

int Graph::getGraphSize() {
    return n;
}

int& Graph::getADJListSize(const int& vertex) {
    return adjListSize[vertex] ;
}

int* Graph::getADJPnt(const int& vertex) {
    return adjIndicator[vertex];
}

double& Graph::getWeight(const int& vertex) {
    return weights[vertex];
}

void Graph::init() {
    adjVertexPtr = new int[2 * m + n];
    memset(adjVertexPtr, 0, (2 * m + n) * sizeof(int));
    adjListSize = new int[n];
    memset(adjListSize, 0, n * sizeof(int));
    adjIndicator = new int*[n];
    memset(adjIndicator, 0, n * sizeof(int *));
    weights = new double[n];
    memset(weights, 0, n * sizeof(double));
}

void Graph::calWeights() {
    for (int i = 0; i < n; i++) {
        weights[i] = (i % 200) + 1;
    }
}

