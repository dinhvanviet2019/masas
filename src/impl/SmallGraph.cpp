/**
 * Name: SmallGraph Implementation
 * Public Funcs: getADJListSize, getADJPnt, getWeight, loadData
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * */
#include "SmallGraph.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>


SmallGraph::SmallGraph(char * file):Graph(file) {
    loadData();
}

void SmallGraph::loadData() {
    
    const int MAX_BUFF_SIZE = 1024;
    char buffer[MAX_BUFF_SIZE];
    FILE * pFile;
    pFile = fopen(filePath, "r"); 
    // read header and skip comment lines       
    while( fgets(buffer, MAX_BUFF_SIZE, pFile) != nullptr) {      
        if (strlen(buffer) == 0 || strchr(buffer, '%') == nullptr) {
            break;
        } else {
            continue;
        }
    };
    
    // read first non-comment line in mtx file (n n m)    
    int meta[3];
    char * pch;
    int count = 0;
    pch = strtok(buffer, " ");    
    while (pch != nullptr) {
        meta[count] = atoi(pch);
        count ++;
        pch = strtok(nullptr, " ");  
    }
    m = meta[2];
    n = meta[0];
    delete pch;
    //printf("%d %d\n", n, m);
    // initialize    
    init();
    // read next m lines which contains edge information
    
    int * tmpPnt = new int[2 * m];
    int u, v;
    int sumTMP = 0;
    for (int i = 0; i < n;i ++) {
        adjListSize[i] = 0;
    }
    for (int i = 0; i < m; i++) {
        fscanf(pFile, "%d %d", &u, &v);
        tmpPnt[i * 2] = u - 1;
        tmpPnt[i * 2 + 1] = v - 1;
        adjListSize[tmpPnt[i * 2]]++;
        adjListSize[tmpPnt[i * 2 + 1]]++;
    }
    
    for (int i =0 ; i < n; i++) {
        sumTMP += adjListSize[i];
    }
    fclose(pFile);
    
    // parse it
    for (int i = 0; i < n; i++) {
        adjListSize[i] ++;
    }
    adjIndicator[0] = adjVertexPtr;
    for (int i = 1; i < n; i++) {
        adjIndicator[i] = adjIndicator[i - 1] + adjListSize[i- 1];
        sumTMP += adjListSize[i - 1];
    }
    int ** tmpIndicator = new int*[n];
    memcpy(tmpIndicator, adjIndicator, n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        *tmpIndicator[i] = i;
        tmpIndicator[i]++;
    }
    
    for (int i = 0; i < m; i++) {
        *tmpIndicator[tmpPnt[2 * i]] = tmpPnt[2 * i + 1];
        tmpIndicator[tmpPnt[2 * i]]++;
        *tmpIndicator[tmpPnt[2 * i + 1]] = tmpPnt[2 * i];
        tmpIndicator[tmpPnt[2 * i + 1]]++;
    }
    
    delete[] tmpPnt;
    delete[] tmpIndicator;
    calWeights();
    
}
