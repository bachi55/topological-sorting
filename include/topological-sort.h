#include <algorithm>
#include <array>
#include <forward_list>
#include <vector>

#include "matrix.h"
typedef Matrix <bool> Graph; 
typedef std::pair <unsigned int, unsigned int> Edge;

class GraphAdjList {
  std::vector <std::forward_list <unsigned int>> _data;
  
  // keep track of the amount of nodes and edges
  const unsigned int _nNodes;
  
  void checkBounds (unsigned int nodeId) const {
    if (nodeId >= _data.size())
      throw std::invalid_argument ("Array index out of bounds.");
  }
  
public:
  GraphAdjList () 
    : _nNodes (0)
  {
    _data = std::vector <std::forward_list <unsigned int>> ();
  }
  
  GraphAdjList (const unsigned int nNodes) 
    : _nNodes (nNodes)
  {
    _data = std::vector <std::forward_list <unsigned int>> (nNodes);
  }
  
  std::forward_list <unsigned int> & operator[] (const unsigned int nodeId) {
    checkBounds (nodeId);
    return this -> _data[nodeId];
  }
  
  const std::forward_list <unsigned int> & operator[] (const unsigned int nodeId) const {
    checkBounds (nodeId);
    return this -> _data[nodeId];
  }
  
  std::vector <std::forward_list <unsigned int>>::iterator begin (void) { return _data.begin(); }
  std::vector <std::forward_list <unsigned int>>::iterator end (void) { return _data.end(); }
  
  std::vector <std::forward_list <unsigned int>>::const_iterator begin (void) const { return _data.begin(); }
  std::vector <std::forward_list <unsigned int>>::const_iterator end (void) const { return _data.end(); }
  
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
  
  // Function which returns true, if no edge is in the graph
  // time-complexity:
  //    O(|V|)
  // TODO: How to keep track of the amount of edges while adding and removing them?
  bool isEmpty (void) const {
    for (auto it = _data.begin(); it != _data.end(); ++it) {
      if (! (it -> empty()))
        return false;
    }
    return true;
  }
 
  unsigned int nNodes (void) const {
    return _nNodes;
  }
  
  void printGraph (std::ostream & ostream = std::cout) const {
    if (! ostream.good())
      throw std::invalid_argument ("Output-stream is not good.");
    
    for (unsigned int sourceNodeId = 0; sourceNodeId < (this -> nNodes()); sourceNodeId++) {
      if (_data[sourceNodeId].empty()) {
        ostream << sourceNodeId << " NULL" << std::endl;
        continue;
      }
      
      const auto adjListBgn = _data[sourceNodeId].begin();
      const auto adjListEnd = _data[sourceNodeId].end();
      
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
std::vector <unsigned int> topologicalSort (Graph graph);

std::vector <unsigned int> topologicalSort (GraphAdjList dag);

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

// Function to check, whether a given topological sorting is valid
//
// time-complexity:
//      O(|V|^2)
bool checkTopologicalSorting (const std::vector <unsigned int> & topologicalSorting, Graph dag);

bool checkTopologicalSorting (const std::vector <unsigned int> & topologicalSorting, const GraphAdjList & dag);

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
unsigned int getMaxNodeId (const std::vector <Edge> edges);

// time-complexity:
//      O(|E|)
GraphAdjList mapFromPosIndecencyToNegIndecency (const GraphAdjList & posIndecencyGraph);