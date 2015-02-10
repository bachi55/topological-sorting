#include <algorithm>
#include <forward_list>
#include <set>
#include <vector>

#include "matrix.h"
#include "GraphAdjList.h"

typedef Matrix <bool> Graph; 
typedef std::pair <unsigned int, unsigned int> Edge;

// IMPLEMENTATIONS OF THE TOPOLOGICAL SORTING
// Function which implements topological sorting for a directed acyclic graph (DAG) [Kahn1962 algorithm]
//
// The graph has to be given as an adjacency matrix.
//       
// time-complexity: 
//      O(|V|^2)
std::vector <unsigned int> topologicalSort (Graph graph);

// Function which implements topological sorting for a directed acyclic graph (DAG) [Kahn1962 algorithm]
//
// The graph has to be given as an adjacency list.
// * posDag: adjacency list of outgoing edges
// * negDag: adjacency list of ingoing edges
//
// time-complexity: 
//      O(|V| * (O(|E|) + log(n))  + |E|), with is the size of the the set keeping the zero-degree nodes, could be O(|V|)
std::vector <unsigned int> topologicalSortAdjList (GraphAdjList posDag, GraphAdjList negDag);

// Function which implements topological sorting for a directed acyclic graph (DAG) [Kahn1962 algorithm]
//
// The graph has to be given as an adjacency list.
// * posDag: adjacency list of outgoing edges
// * negDag: adjacency list of ingoing edges
//
// time-complexity: 
//      O(|V| * log(n) + |E|), with is the size of the the set keeping the zero-degree nodes, could be O(|V|)
std::vector <unsigned int> topologicalSortAdjList2 (GraphAdjList posDag, GraphAdjList negDag);

// Function which implements topological sorting for a directed acyclic graph (DAG) [Kahn1962 algorithm]
//
// The graph has to be given as an adjacency list.
//
// time-complexity: 
//      O(|V| + |E|)
std::vector <unsigned int> topologicalSortAdjList3 (GraphAdjList dag);

// Function which implements topological sorting for a directed acyclic graph (DAG) [Corman algorithm]
//
// The graph has to be given as an adjacency list.
//
// time-complexity: 
//      O(|V| + |E|)
std::vector <unsigned int> topologicalSortCormanAdjList (GraphAdjList posDag);

std::vector <unsigned int> topologicalSortCormanAdjList2 (const GraphAdjList & posDag);

// HELPER FUNCTION FOR THE SORTING ALGORITHMS
// Function to check, whether a given vertex has an incoming edge
//
// time-complexity: 
//      O(|V|)
bool hasIncommingEdges (const Graph & dag, unsigned int vertex);

// Function to check, whether a given directed acyclic graph contains edges or not
//
// time-complexity:
//      O(|V|^2)
bool hasDAGEdges (const Graph & dag);

// Function to calculate the amount of incoming edges for each node in a given DAG
//
// time-complexity:
//      O(|E|)
std::vector <unsigned int> getInDegree (const GraphAdjList & dag);

// time-complexity: ?
void visit (const unsigned int sourceNodeId, GraphAdjList & posDag, std::vector<unsigned int> & L, std::set <unsigned int> & unmarkedNodes);

void visit2 (const unsigned int sourceNodeId, const GraphAdjList & posDag, std::vector <unsigned int> & L, std::set <unsigned int> & unmarkedNodes, unsigned int & nodeCounter, std::vector <NodeColor> & nodeColors);

// FUNCTIONS TO CHECK A GIVEN TOPOLOGICAL SORTING FOR CORRECTNESS
// Function to check, whether a given topological sorting is valid
//
// time-complexity:
//      O(|V|^2)
bool checkTopologicalSorting (const std::vector <unsigned int> & topologicalSorting, Graph dag);

// Function to check, whether a given topological sorting is valid
//
// time-complexity:
//      O(|V| * |E|)
bool checkTopologicalSorting (const std::vector <unsigned int> & topologicalSorting, const GraphAdjList & dag);

// FUNCTIONS TO READ GRAPHS FROM FILES AND CREATE REPRESENTATIONS TO PROCESS THEM
// Function to read a directed edges from a file
std::vector <Edge> readEdgesFromFile (const std::string & filename);

void readEdgesFromFile (const std::string & filename, std::vector <Edge> & posEdges, std::vector <Edge> & negEdges);

// Function to create a directed graph (matrix) from a vector of given edges
//
// time-complexity:
//      O(|E|)
Graph createGraphFromEdges (const std::vector <Edge> & edges);

// Function to create a directed graph (adjacency list) from a vector of given edges
//
// time-complexity:
//      O(|E|)
GraphAdjList createGraphAdjListFromEdges (const std::vector <Edge> & edges);

// time-complexity:
//      O(|E|)
GraphAdjList mapFromPosIndecencyToNegIndecency (const GraphAdjList & posIndecencyGraph);

// Function to determine the largest node-id in a given vector of edges
//
// time-complexity:
//      O(|E|)
unsigned int getMaxNodeId (const std::vector <Edge> edges);

