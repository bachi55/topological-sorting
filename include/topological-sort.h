#include <vector>

#include "matrix.h"
typedef Matrix <float> Graph; 

// Function which implements topological sorting for a directed acyclic graph (DAG)
// in: 
//      Graph as adjacency matrix
// out:
//      indeces of graph nodes in a valid topological order
//      NOTE: If you have to make a copy (here graph is a copy, since it is a by-value
//            parameter) let the compiler do it. 
std::vector <size_t> topologicalSort (Graph graph);

// Function to check, whether a given vertex has an incomming edge
bool hasIncommingEdges (const Graph & dag, size_t vertex);

// Function to check, whether a given directed acyclic graph contains edges or not
bool hasDAGEdges (const Graph & dag);

// Function to check, whether a given topological sorting is valid
bool checkTopologicalSorting (const std::vector <size_t> & topologicalSorting, Graph dag);