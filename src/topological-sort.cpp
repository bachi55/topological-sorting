#include "topological-sort.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <streambuf>
#include <sstream>


std::vector <unsigned int> topologicalSort (Graph dag) {
  // check whether the given matrix can be an adjacency matrix
  if (dag.cols() != dag.rows())
    throw std::invalid_argument ("Adjacency matrix is not quadratic and therefor not valid.");
  
  // if the matrix is empty, it is considered to be a valid DAG and an empty list is
  // returned
  if (dag.isEmpty())
    return std::vector <unsigned int> ();
  
  // list which will contain the sorted vertex-indices
  std::vector <unsigned int> L;

  // set of vertices with no incomming edges
  std::set <unsigned int> S;
  for (unsigned int n = 0; n < dag.rows(); n++)
    if (! hasIncommingEdges (dag, n))
      S.insert (n);
    
  while (! S.empty()) {
    // NOTE: The sorting is not unique. The strategy of removing the vertices from 
    //       S is determining the order of the sorting.
    auto n = *S.begin();
    L.push_back (n);
    // remove n from S
    S.erase (S.begin());
    
    // check the adjacency-row for the current vertex    
    for (unsigned int m = 0; m < dag.cols(); m++) {
      // check whether there is an egde e from n to m
      if (dag(n, m) != 0) {
        // delete edge e from DAG
        dag(n, m) = 0;
        // check whether m has incomming edges
        if (! hasIncommingEdges (dag, m))
          S.insert (m);
      }
    }
  }
  
  // if the graph still has edges, there has been a cycle
  if (hasDAGEdges (dag))
    throw std::invalid_argument ("The given graph is not (D)irected (A)cyclic (G)raph");
  
  return L;
}

std::vector <unsigned int> topologicalSort (GraphAdjList dag) {
  auto negDag = mapFromPosIndecencyToNegIndecency (dag);
  
  // if the matrix is empty, it is considered to be a valid DAG and an empty list is
  // returned
  if (dag.isEmpty())
    return std::vector <unsigned int> ();
  
  // list which will contain the sorted vertex-indices
  std::vector <unsigned int> L;
  
  // set of vertices with no incomming edges
  std::set <unsigned int> S;
  for (size_t sourceNodeId = 0; sourceNodeId < negDag.nNodes(); sourceNodeId++)
    if (negDag[sourceNodeId].empty())
      S.insert (sourceNodeId);
    
  while (! S.empty()) {
    // NOTE: The sorting is not unique. The strategy of removing the vertices from 
    //       S is determining the order of the sorting.
    auto n = *S.begin();
    L.push_back (n);
    // remove n from S
    S.erase (S.begin());
    
    // get the adjacency list for the current vertex and delete all edges from n to m
    while (! dag[n].empty()) {
      // delete n as precondition (incoming edge) from m
      negDag.deleteEdge (Edge (dag[n].front(), n));
      // check whether m has still incoming edges
      if (negDag[dag[n].front()].empty())
        S.insert (dag[n].front());
      // delete edge e from DAG
      dag[n].pop_front();
    }
  }
  
  // if the graph still has edges, there has been a cycle
  if (! dag.isEmpty())
    throw std::invalid_argument ("The given graph is not (D)irected (A)cyclic (G)raph");
  
  return L;    
}

bool hasIncommingEdges (const Graph & dag, unsigned int vertexInd) {
  for (unsigned int sourceVertexId = 0; sourceVertexId < dag.rows(); sourceVertexId++) 
    if (dag(sourceVertexId, vertexInd) != 0) 
      return true;
    
  return false;
}

bool hasDAGEdges (const Graph & dag) {
  for (unsigned int i = 0; i < dag.rows(); i++) for (unsigned int j = 0; j < dag.cols(); j++)
    if (dag(i, j) != 0) 
      return true;
      
  return false;
}

bool checkTopologicalSorting (const std::vector <unsigned int> & topologicalSorting, Graph dag) {
  // check whether the given matrix can be an adjacency matrix
  if (dag.cols() != dag.rows())
    throw std::invalid_argument ("Adjacency matrix is not quadratic and therefor not valid.");
  
  // check whether the sorting contain enought nodes
  if (dag.rows() != topologicalSorting.size())
    throw std::invalid_argument ("The topological sorting and the matrix does not fit considering there dimension");
  
  for (auto node = topologicalSorting.begin(); node != topologicalSorting.end(); ++node) {
    // if there are incomming edges there are preconditions, which has not been done 
    if (hasIncommingEdges (dag, *node)) 
      return false;
    
    // delete the current not from all the not done preconditions
    for (unsigned int i = 0; i < dag.cols(); i++)
      dag (*node, i) = 0;
  }
  
  return true;
}

bool checkTopologicalSorting (const std::vector <unsigned int> & topologicalSorting, const GraphAdjList & dag) {
  // check whether the sorting contain enough nodes
  if (dag.nNodes() != topologicalSorting.size())
    throw std::invalid_argument ("The topological sorting and the adjacency list does not fit considering there dimension");
  
  auto negDag = mapFromPosIndecencyToNegIndecency (dag);
  
  for (auto node = topologicalSorting.begin(); node != topologicalSorting.end(); ++node) {
    // If a node has incoming edged, than there are preconditions (other nodes), which has not been done (deleted) till now.
    if (! negDag[*node].empty())
      return false;
    
    // Delete the current node to indicate, that it is now precondition anymore. To do this, all incoming edges outgoing from the current node has to be deleted.
    for (unsigned int preconditionNodeId = 0; preconditionNodeId < negDag.nNodes(); preconditionNodeId++) {
      negDag.deleteEdge (Edge (preconditionNodeId, *node));
    }
  }
  
  return true;
}

std::vector <Edge> readEdgesFromFile (const std::string & filename) {
  std::ifstream inFile (filename);
  if (! inFile) 
    throw std::invalid_argument ("Cannot open file: " + filename);

  std::vector <Edge> edges; 
  unsigned int sourceNodeId, targetNodeId;
  // NOTE: >> uses WHITESPACE and NEWLINE as delimiters 
  while (inFile >> sourceNodeId >> targetNodeId) {
    edges.push_back (Edge (sourceNodeId, targetNodeId));
    
//     std::cout << "add edge to graph:" << std::endl
//               << "(" << edges.back().first << ")"
//               << " --> "
//               << "(" << edges.back().second << ")"
//               << std::endl;
  }
    
  inFile.close();
  
  return edges;
}

unsigned int getMaxNodeId (const std::vector <Edge> edges) {
  unsigned int maxNodeId = 0;
  for (auto & edge : edges)
    maxNodeId = std::max (maxNodeId, std::max (edge.first, edge.second));
  return maxNodeId;
}

Graph createGraphFromEdges (const std::vector <Edge> & edges) {
  if (edges.size() < 1)
    return Graph();
  
  auto maxNodeId = getMaxNodeId (edges);
  
  // initialize a empty graph
  // NOTE: a node can have id 0
  Graph graph (maxNodeId + 1, maxNodeId + 1, false);
  
  std::for_each (edges.begin(), edges.end(), [&graph](Edge e) {
    graph(e.first, e.second) = 1;
  });
  
  return graph;
}

GraphAdjList createGraphAdjListFromEdges (const std::vector <Edge> & edges) {
  if (edges.size() < 1)
    return GraphAdjList();
  
  auto maxNodeId = getMaxNodeId (edges);
  
  // initialize a empty graph
  // NOTE: a node can have id 0
  GraphAdjList graph (maxNodeId + 1);
  
  std::for_each (edges.begin(), edges.end(), [&graph](Edge e) {
    graph.insertEdge (e, true);
  });
  
  return graph;
}

GraphAdjList mapFromPosIndecencyToNegIndecency (const GraphAdjList & posIndecencyGraph) {
  if (posIndecencyGraph.isEmpty())
    return GraphAdjList();
  
  GraphAdjList negIndecencyGraph (posIndecencyGraph.nNodes());
  for (unsigned int sourceNodeId = 0; sourceNodeId < posIndecencyGraph.nNodes(); sourceNodeId++) {
    if (posIndecencyGraph[sourceNodeId].empty())
      continue;
    
    auto bgn = posIndecencyGraph[sourceNodeId].begin();
    auto end = posIndecencyGraph[sourceNodeId].end();
    
    for (auto targetNode = bgn; targetNode != end; ++targetNode)
      negIndecencyGraph.insertEdge (Edge (*targetNode, sourceNodeId));
  }
  
  return negIndecencyGraph;
}
 

