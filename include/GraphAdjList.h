#include <forward_list>
#include <vector>

#include "matrix.h"
typedef Matrix <bool> Graph; 
typedef std::pair <unsigned int, unsigned int> Edge;

enum struct NodeColor {UNMARKED, TEMPORARILY_MARKED, PERMANENTLY_MARKED};

// This class can be used to represent a graph as a adjacency list
class GraphAdjList {
  
  // adjacency list
  std::vector <std::forward_list <unsigned int>> _data;
  // node colors 
  std::vector <NodeColor> _nodeColors;
  
  // keep track of the amount of nodes and edges
  const unsigned int _nNodes;
  
  // function to check, whether a given node id is valid 
  inline void checkBounds (unsigned int nodeId) const {
    if (nodeId >= _data.size())
      throw std::invalid_argument ("Array index out of bounds.");
  }
  
public:
  
  // Constructors
  GraphAdjList () 
    : _nNodes (0)
  {
    _data = std::vector <std::forward_list <unsigned int>> ();
    _nodeColors = std::vector <NodeColor> ();
  }
  
  GraphAdjList (const unsigned int nNodes) 
    : _nNodes (nNodes)
  {
    _data = std::vector <std::forward_list <unsigned int>> (nNodes);
    _nodeColors = std::vector <NodeColor> (nNodes, NodeColor::UNMARKED);
  }
  
  // Access-operators
  std::forward_list <unsigned int> & operator[] (const unsigned int nodeId) {
    checkBounds (nodeId);
    return this -> _data[nodeId];
  }
  
  const std::forward_list <unsigned int> & operator[] (const unsigned int nodeId) const {
    checkBounds (nodeId);
    return this -> _data[nodeId];
  }
  
  // iterators
  std::vector <std::forward_list <unsigned int>>::iterator begin (void) { return _data.begin(); }
  std::vector <std::forward_list <unsigned int>>::iterator end (void) { return _data.end(); }
  
  std::vector <std::forward_list <unsigned int>>::const_iterator begin (void) const { return _data.begin(); }
  std::vector <std::forward_list <unsigned int>>::const_iterator end (void) const { return _data.end(); }
  
  // access and set the node colors
  inline NodeColor getNodeColor (const unsigned int nodeId) const {
    checkBounds (nodeId);
    return _nodeColors[nodeId];
  }
  
  inline void setNodeColor (const unsigned int nodeId, const NodeColor newNodeColor) {
    checkBounds (nodeId);
    _nodeColors[nodeId] = newNodeColor;
  }
  
  // Function to determine, whether is given edge is within the adjacency list
  // 
  // time-complexity:
  //    O(|E|)
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
  
  // Function to delete a given edge from an adjacency list
  //
  // time-complexity:
  //    O(|outgoing edges from e.first|)
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
 
  // Function to give the number of nodes in the adjacency list
  inline unsigned int nNodes (void) const {
    return _nNodes;
  }
  
  // Function to output a adjacency list to an output stream
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