/**
 * Name: Graph Header
 * Purpose: store graph contains: vertex, edges, weight of vertices
 * Author: Viet-DinhVan (loveyou)
 * Created at: 2021-01-17
 * History:
 *      1. 2021-01-17 initialize
 * Limitation: only support for MTX
 * */
#ifndef GRAPH_H
#define GRAPH_H
class Graph {
public:
    Graph(char * file); // create Graph Instance from data file path
    ~Graph(); // destructor
    int getGraphSize();
    int& getADJListSize(const int& vertex); // get neighborhood vertex size of a vertex
    int* getADJPnt(const int& vertex); // get the pointer locating the head of adjacent list memory of a vertex
    double& getWeight(const int& vertex); // get weight of the vertex
protected:    
    virtual void loadData() = 0; // load data into graph
    void init(); // initialize value for data member of the instance
    void calWeights(); // calculate value of weight data member of the instance
    char filePath[1024]; // file path of data 
    int n; // number of vertices
    int m; // number of edges
    int* adjVertexPtr; // vector of adjacent vertices array
    int* adjListSize;// vector of size of each neighbor vertex list of a vertex
    int** adjIndicator; // vector of points that point to the head of each adjacent vertices of a vertex    
    double* weights; // vector of weight of vertices
};
#endif