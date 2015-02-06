#include <algorithm>
#include <array>
#include <forward_list>
#include <vector>

#include "matrix.h"
typedef Matrix <bool> Graph; 
typedef std::pair <size_t, size_t> Edge;

class GraphAdjList {
  std::vector <std::forward_list <size_t>> _data;
  
  void checkBounds (size_t nodeId) const {
    if (nodeId >= _data.size())
      throw std::invalid_argument ("Array index out of bounds.");
  }
  
public:
  GraphAdjList () {
    _data = std::vector <std::forward_list <size_t>> ();
  }
  
  GraphAdjList (const size_t nNodes) {
    _data = std::vector <std::forward_list <size_t>> (nNodes);
  }
  
  std::forward_list <size_t> & operator[] (const size_t nodeId) {
    checkBounds (nodeId);
    return _data[nodeId];
  }
  
  std::forward_list <size_t> operator[] (const size_t nodeId) const {
    checkBounds (nodeId);
    return _data[nodeId];
  }
  
  
  bool containsEdge (const Edge & e) const {
    checkBounds (e.first);
    checkBounds (e.second);
    
    const auto adjListBgn = _data[e.first].begin();
    const auto adjListEnd = _data[e.first].end();
    for (auto nodeIt = adjListBgn; nodeIt != adjListEnd; ++nodeIt)
      if ((*nodeIt) == e.second)
        return true;
      
    return false;
  }
  
  void deleteEdge (const Edge & e) {
    checkBounds (e.first);
    checkBounds (e.second);
    _data[e.first].remove (e.second); 
  }
  
  // time-complexity:
  //    O(1) ... if 'removeDoubleEdges' is false
  //    O(|outgoing edges from e.first|) ... else
  void insertEdge (const Edge & e, const bool removeDoubleEdges = true) {
    checkBounds (e.first);
    checkBounds (e.second);
    _data[e.first].push_front (e.second);
    
    if (removeDoubleEdges)
      _data[e.first].unique();
  }
  
  void printGraph (std::ostream & ostream = std::cout) {
    if (! ostream.good())
      throw std::invalid_argument ("Output-stream is not good.");
    
    for (size_t sourceNodeId = 0; sourceNodeId < _data.size(); sourceNodeId++) {
      if (_data[sourceNodeId].empty()) {
        ostream << sourceNodeId << " NULL" << std::endl;
        continue;
      }
      
      auto adjListBgn = _data[sourceNodeId].begin();
      auto adjListEnd = _data[sourceNodeId].end();
      
      for (auto targetNode = adjListBgn; targetNode != adjListEnd; ++targetNode)
        ostream << sourceNodeId << " " << *targetNode << std::endl;
    }
  }
};

// Function to create a directed graph (adjacency list) from a vector of given edges
//
// time-complexity:
//      O(|E|)
GraphAdjList createGraphAdjListFromEdges (const std::vector <Edge> & edges);

// Function which implements topological sorting for a directed acyclic graph (DAG)
// in: 
//      Graph as adjacency matrix
// out:
//      indexes of graph nodes in a valid topological order
//      NOTE: If you have to make a copy (here graph is a copy, since it is a by-value
//            parameter) let the compiler do it. 
// 
// time-complexity: 
//      O(|V|^2)
std::vector <size_t> topologicalSort (Graph graph);

// Function to check, whether a given vertex has an incoming edge
//
// time-complexity: 
//      O(|V|)
bool hasIncommingEdges (const Graph & dag, size_t vertex);

// Function to check, whether a given directed acyclic graph contains edges or not
//
// time-complexity:
//      O(|V|^2)
bool hasDAGEdges (const Graph & dag);

// Function to check, whether a given topological sorting is valid
//
// time-complexity:
//      O(|V|^2)
bool checkTopologicalSorting (const std::vector <size_t> & topologicalSorting, Graph dag);

// Function to read a directed edges from a file
std::vector <Edge> readEdgesFromFile (const std::string & filename);

// Function to create a directed graph (matrix) from a vector of given edges
//
// time-complexity:
//      O(|E|)
Graph createGraphFromEdges (const std::vector <Edge> & edges);

// Function to determine the largest node-id in a given vector of edges
//
// time-complexity:
//      O(|E|)
size_t getMaxNodeId (const std::vector <Edge> edges);