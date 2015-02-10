#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <cstdlib>

#include "meter.h"
#include "topological-sort.h"


// function to print a vector
template <typename T>
void printVector (const std::vector <T> & v, const std::string & prefix = "") {
  std::cout << prefix;
  for (auto it = v.begin(); it != v.end(); ++it)
    std::cout << (*it) << " ";
  std::cout << std::endl;
};

// function to check, whether a file exists
inline bool isFileExisting (const std::string & name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

void fillWith2ndPower (const unsigned int from, const unsigned int to, std::vector <unsigned int> & oVector) {
  if (to < from) 
    return;
  
  oVector.clear();
  for (unsigned int i = from; i < to; i++)
    oVector.push_back (pow (2, i));
}

std::string createRandomDAG (const float epsilon,  unsigned int seed = 1) {
  std::string dagFilename = "/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/test-graph.dat";
  
  unsigned int nNodes = rand() % 512;
  
  char buffer[2048];
  sprintf (buffer
  , "Rscript scripts/createRandomDAG.R %i %.1f %s %i"
  , nNodes
  , epsilon
  , dagFilename.c_str()
  , seed);
  if (system ((char *) buffer))
    throw std::invalid_argument ("Something went wrong in the R script.");
  
  return dagFilename;
}

// test I/O routines
TEST (correctness, readGraphFromFile) {
  auto edges = readEdgesFromFile ("example-graphs/t1-graph.dat");
  
  auto graphAdjMatrix = createGraphFromEdges (edges);
  ASSERT_EQ (graphAdjMatrix.isEmpty(), false);
//   graphAdjMatrix.printMatrix();
  
  auto graphAdjList = createGraphAdjListFromEdges (edges);
  ASSERT_EQ (graphAdjList.nNodes(), 7);
//   graphAdjList.printGraph();
  
  std::vector <Edge> posEdges, negEdges;
  readEdgesFromFile ("example-graphs/t1-graph.dat", posEdges, negEdges);
  
  ASSERT_EQ (posEdges.size() > 0, true);
  ASSERT_EQ (negEdges.size() > 0, true);
  
  auto posDagAdjList = createGraphAdjListFromEdges (posEdges);
  auto negDagAdjList = createGraphAdjListFromEdges (negEdges);
  
  ASSERT_EQ (posDagAdjList.isEmpty(), false);
  ASSERT_EQ (negDagAdjList.isEmpty(), false);
  
//   std::cout << "pos:" << std::endl;
//   posDagAdjList.printGraph();
//   std::cout << "neg:" << std::endl;
//   negDagAdjList.printGraph();
}

// test adjList code
TEST (correctness, adjList_containsEdge) {
  {
    std::vector <Edge> edges ({Edge (0, 1)
                            , Edge (0, 2)
                            , Edge (1, 3)
                            , Edge (2, 3)
                            , Edge (1, 4)});
    auto graphAdjList = createGraphAdjListFromEdges (edges);
    
    ASSERT_EQ (graphAdjList.containsEdge (Edge (1, 0)), false);
    ASSERT_EQ (graphAdjList.containsEdge (Edge (2, 3)), true);
    ASSERT_EQ (graphAdjList.containsEdge (Edge (0, 0)), false);
    ASSERT_EQ (graphAdjList.containsEdge (Edge (0, 1)), true);
    ASSERT_EQ (graphAdjList.containsEdge (Edge (0, 2)), true);
  }
  
  {
    std::vector <Edge> edges;
    auto graphAdjList = createGraphAdjListFromEdges (edges);
    
    // works correctly!
//     ASSERT_EQ (graphAdjList.containsEdge (Edge (0, 0)), false);
    graphAdjList.printGraph();
    
    // works correctly!
//     graphAdjList.insertEdge (Edge (0, 1));
  }
}

TEST (correctness, adjList_nNodes_isEmpty) {
  {
    GraphAdjList graph;
    ASSERT_EQ (graph.isEmpty(), true);
    ASSERT_EQ (graph.nNodes(), 0);
  }
  {
    GraphAdjList graph (0);
    ASSERT_EQ (graph.isEmpty(), true);
    ASSERT_EQ (graph.nNodes(), 0);
  }
  {
    std::vector <Edge> edges ({Edge (0, 1)
                             , Edge (0, 2)
                             , Edge (1, 3)
                             , Edge (2, 3)
                             , Edge (1, 4)});
    auto graph = createGraphAdjListFromEdges (edges);
    
    ASSERT_EQ (graph.isEmpty(), false);
    ASSERT_EQ (graph.nNodes(), 5);

    Edge e;
    e = Edge (0, 1); graph.deleteEdge (e);
    e = Edge (0, 2); graph.deleteEdge (e);
    e = Edge (1, 3); graph.deleteEdge (e);
    e = Edge (2, 3); graph.deleteEdge (e);
    ASSERT_EQ (graph.isEmpty(), false);
    
    e = Edge (1, 4); graph.deleteEdge (e);
    ASSERT_EQ (graph.isEmpty(), true);
    ASSERT_EQ (graph.nNodes(), 5);
  }
  
}
  
TEST (correctness, adjList_delete_insert_Edge) {
  std::vector <Edge> edges ({Edge (0, 1)
                           , Edge (0, 2)
                           , Edge (1, 3)
                           , Edge (2, 3)
                           , Edge (1, 4)});
  auto graphAdjList = createGraphAdjListFromEdges (edges);
  
  Edge e (0, 1);
  ASSERT_EQ (graphAdjList.containsEdge (e), true);
  std::cout << "delete edge: (" << e.first << ") --> (" << e.second << ")" << std::endl;
  graphAdjList.deleteEdge (e);
  ASSERT_EQ (graphAdjList.containsEdge (e), false);
  
  e = Edge (0, 1);
  ASSERT_EQ (graphAdjList.containsEdge (e), false);
  std::cout << "insert edge: (" << e.first << ") --> (" << e.second << ")" << std::endl;
  graphAdjList.insertEdge (e);
  ASSERT_EQ (graphAdjList.containsEdge (e), true);
  
  e = Edge (2, 4);
  ASSERT_EQ (graphAdjList.containsEdge (e), false);
  std::cout << "insert edge: (" << e.first << ") --> (" << e.second << ")" << std::endl;
  graphAdjList.insertEdge (e);
  ASSERT_EQ (graphAdjList.containsEdge (e), true);
  
  e = Edge (0, 1); graphAdjList.deleteEdge (e);
  e = Edge (0, 2); graphAdjList.deleteEdge (e);
  e = Edge (1, 3); graphAdjList.deleteEdge (e);
  e = Edge (2, 3); graphAdjList.deleteEdge (e);
  e = Edge (2, 4); graphAdjList.deleteEdge (e);
  e = Edge (1, 4); graphAdjList.deleteEdge (e);
  
  graphAdjList.printGraph();
}

TEST (correctness, adjList_map_pos_to_neg_indecency) {
  std::vector <Edge> edges ({Edge (0, 1)
                           , Edge (0, 2)
                           , Edge (1, 3)
                           , Edge (2, 3)
                           , Edge (1, 4)});
  auto graphAdjList = createGraphAdjListFromEdges (edges);
  
//   std::cout << "positive indecency list:" << std::endl;
//   graphAdjList.printGraph();
  
  auto negIndecencyList = mapFromPosIndecencyToNegIndecency (graphAdjList);
  
//   std::cout << "negative indecency list:" << std::endl;
//   negIndecencyList.printGraph();
  
  ASSERT_EQ (negIndecencyList.isEmpty(), graphAdjList.isEmpty());
  ASSERT_EQ (negIndecencyList.nNodes(), graphAdjList.nNodes());
  
  ASSERT_EQ (negIndecencyList.containsEdge (Edge (1, 0)), true);
  ASSERT_EQ (negIndecencyList.containsEdge (Edge (2, 0)), true);
  ASSERT_EQ (negIndecencyList.containsEdge (Edge (3, 1)), true);
  ASSERT_EQ (negIndecencyList.containsEdge (Edge (3, 2)), true);
  ASSERT_EQ (negIndecencyList.containsEdge (Edge (4, 1)), true);
}

TEST (correctness, adjList_checkTopologicalSorting) {
  {
    auto dag = createGraphAdjListFromEdges (readEdgesFromFile ("example-graphs/t1-graph.dat"));
    ASSERT_EQ (checkTopologicalSorting ({5, 6, 4, 3, 2, 0, 1}, dag), true);
    ASSERT_EQ (checkTopologicalSorting ({1, 6, 4, 3, 2, 0, 5}, dag), false);
  }
  {
    auto dag = createGraphAdjListFromEdges (std::vector <Edge> ());
    ASSERT_EQ (checkTopologicalSorting ({}, dag), true);
  }
  {
    auto dag = createGraphAdjListFromEdges (std::vector <Edge> ({Edge (1, 2)}));
    ASSERT_EQ (checkTopologicalSorting ({0, 1, 2}, dag), true);
    ASSERT_EQ (checkTopologicalSorting ({0, 2, 1}, dag), false);
  }
  {
    auto dag = createGraphAdjListFromEdges (std::vector <Edge> ({
        Edge (1, 2)
      , Edge (2, 3)
      , Edge (1, 3)
    }));
    ASSERT_EQ (checkTopologicalSorting ({0, 1, 2, 3}, dag), true);
    ASSERT_EQ (checkTopologicalSorting ({1, 2, 3, 0}, dag), true);
    ASSERT_EQ (checkTopologicalSorting ({0, 3, 1, 2}, dag), false);
  }
  {
    auto dag = createGraphAdjListFromEdges (std::vector <Edge> ({
        Edge (1, 2)
      , Edge (2, 3)
      , Edge (1, 3)
      , Edge (2, 4)
      , Edge (0, 1)
      , Edge (0, 4)
    }));
    ASSERT_EQ (checkTopologicalSorting ({0, 1, 2, 3, 4}, dag), true);
    ASSERT_EQ (checkTopologicalSorting ({0, 4, 1, 2, 3}, dag), false);
  }
}

TEST (correctness, adjList_coloring) {
  GraphAdjList graph (10);
  
  ASSERT_EQ (graph.getNodeColor(0), NodeColor::UNMARKED);
  ASSERT_EQ (graph.getNodeColor(1), NodeColor::UNMARKED);
  
  graph.setNodeColor(0, NodeColor::PERMANENTLY_MARKED);
  ASSERT_EQ (graph.getNodeColor(0), NodeColor::PERMANENTLY_MARKED);
}

// test adjMatrix code
TEST (correctness, hasIncommingEdges) {
  {
    Graph dag;
    ASSERT_EQ (hasIncommingEdges (dag, 0), false);
  }
  
  {
    Graph dag (1, 1, {0});
    ASSERT_EQ (hasIncommingEdges (dag, 0), false);
  }
  
  {
    Graph dag (1, 1, {1});
    ASSERT_EQ (hasIncommingEdges (dag, 0), true);
  }
  
  {
    Graph dag (2, 2, {0, 1, 0, 0});
    ASSERT_EQ (hasIncommingEdges (dag, 1), true);
    ASSERT_EQ (hasIncommingEdges (dag, 0), false);
  }
  
  {
    Graph dag (2, 2, {1, 0, 1, 0});
    ASSERT_EQ (hasIncommingEdges (dag, 0), true);
    ASSERT_EQ (hasIncommingEdges (dag, 1), false);
  }
  
  {
    Graph dag (2, 2, {0, 0, 1, 0});
    ASSERT_EQ (hasIncommingEdges (dag, 0), true);
    ASSERT_EQ (hasIncommingEdges (dag, 1), false);
  }
}

TEST (correctness, hasDAGEdges) {
  {
    Graph dag;
    ASSERT_EQ (hasDAGEdges (dag), false);
  }
  
  {
    Graph dag (1, 1, {0});
    ASSERT_EQ (hasDAGEdges (dag), false);
  }
  
  {
    Graph dag (1, 1, {1});
    ASSERT_EQ (hasDAGEdges (dag), true);
  }
  
  {
    Graph dag (2, 2, {0, 1, 0, 0});
    ASSERT_EQ (hasDAGEdges (dag), true);
  }
  
  {
    Graph dag (2, 2, {1, 0, 1, 0});
    ASSERT_EQ (hasDAGEdges (dag), true);
  }
  
  {
    Graph dag (2, 2, {0, 0, 0, 0});
    ASSERT_EQ (hasDAGEdges (dag), false);
  }  
}

TEST (correctness, checkTopologicalSorting) {
  
  {
    Graph dag;
    
    ASSERT_EQ (checkTopologicalSorting ({}, dag), true);
  }
  
  {
    Graph dag (2, 2, {
      0, 1,
      0, 0
    });
    
    ASSERT_EQ (checkTopologicalSorting ({0, 1}, dag), true);
  }
  
  {
    Graph dag (2, 2, {
      0, 0,
      1, 0
    });

    ASSERT_EQ (checkTopologicalSorting ({1, 0}, dag), true);
  }
  
  {
    Graph dag (3, 3, {
      0, 1, 1,
      0, 0, 1,
      0, 0, 0
    });
      
    ASSERT_EQ (checkTopologicalSorting ({0, 1, 2}, dag), true);
    ASSERT_EQ (checkTopologicalSorting ({1, 2, 0}, dag), false);
  }
  
  {
    Graph dag (3, 3, {
      0, 1, 1,
      0, 0, 1,
      1, 0, 0
    });
    
    ASSERT_EQ (checkTopologicalSorting ({0, 1, 2}, dag), false);
  }
  
  {
    Graph dag (3, 3, {
      0, 1, 1,
      0, 0, 1,
      1, 0, 0
    });
    
    ASSERT_EQ (checkTopologicalSorting ({0, 1, 2}, dag), false);
  }
  
  {
    Graph dag (4, 4, {
      0, 1, 1, 1,
      0, 0, 1, 0,
      0, 0, 0, 1,
      0, 0, 0, 0
    });
    
    ASSERT_EQ (checkTopologicalSorting ({0, 1, 2, 3}, dag), true);
  }
  
  {
    Graph dag = createGraphFromEdges (readEdgesFromFile ("example-graphs/t1-graph.dat"));
    ASSERT_EQ (checkTopologicalSorting ({5, 6, 4, 3, 2, 0, 1}, dag), true);
    ASSERT_EQ (checkTopologicalSorting ({1, 6, 4, 3, 2, 0, 5}, dag), false);
  }
  
}

// test different sorting implementations
// TEST (correctness, topologicalSortAdjMatrix) {
//   // NOTE: There is topological-sort implementation in linux, which could
//   //       serve as reference. Check: http://en.wikipedia.org/wiki/Tsort
//   //       and 'tsort' in the terminal.
//   
//   {
//     Graph dag;
//     
//     auto L = topologicalSort (dag);
//     ASSERT_EQ (checkTopologicalSorting (L, dag), true);
//   }
//   
//   {
//     Graph dag (2, 2, {
//       0, 1,
//       0, 0
//     });
//     
//     auto L = topologicalSort (dag);
//     ASSERT_EQ (checkTopologicalSorting (L, dag), true);
//   }
//   
//   {
//     Graph dag (2, 2, {
//       0, 0,
//       1, 0
//     });
//     
//     auto L = topologicalSort (dag);
//     ASSERT_EQ (checkTopologicalSorting (L, dag), true);
//   }
//   
//   {
//     Graph dag (3, 3, {
//       0, 1, 1,
//       0, 0, 1,
//       0, 0, 0
//     });
//     
//     auto L = topologicalSort (dag);
//     ASSERT_EQ (checkTopologicalSorting (L, dag), true);
//   }
//   
//   {
//     // NOTE: This graph comes from: http://en.wikipedia.org/wiki/Topological_sorting
//     Graph dag (11, 11, {
//       //  1  2  3  4  5  6  7  8  9 10 11
//       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1 * 
//       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2 * 
//       0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, // 3
//       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4 * 
//       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, // 5
//       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6 *
//       0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, // 7
//       0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, // 8 
//       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9 *
//       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10 *
//       0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0  // 11
//     });
//     
//     auto L = topologicalSort (dag);
//     ASSERT_EQ (checkTopologicalSorting (L, dag), true);
//   }
//   
//   {
//     Graph dag (4, 4, {
//       0, 1, 1, 1,
//       0, 0, 1, 0,
//       0, 0, 0, 1,
//       0, 0, 0, 0
//     });
//     
//     auto L = topologicalSort (dag);
//     ASSERT_EQ (checkTopologicalSorting (L, dag), true);
//   }
//   
//   {
//     std::string filename = "/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/test-graph.dat";
//     
//     for (unsigned int i = 0; i < 5; i++) {
//       std::cout << "sort random DAG " << (i + 1) << "/5" << std::endl;
//       // create a random DAG
//       char buffer[2048];
//       sprintf (buffer
//       , "Rscript scripts/createRandomDAG.R 200 0.6 %s %i"
//       , filename.c_str()
//       , i);
//       if (system ((char *) buffer)) 
//         std::cout << "something went wrong in the R script" << std::endl;
//       
//       Graph dag = createGraphFromEdges (readEdgesFromFile (filename));
//       ASSERT_EQ (checkTopologicalSorting (topologicalSort (dag), dag), true);
//     }
//   }
//   
// }

// TEST (correctness, topologicalSortAdjList) {
//   {
//     GraphAdjList posDag;
//     
//     auto L = topologicalSortAdjList (posDag, posDag);
//     ASSERT_EQ (checkTopologicalSorting (L, posDag), true);
//   }
//   
//   {
//     std::string filename = "/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/test-graph.dat";
//     
//     for (unsigned int i = 0; i < 20; i++) {
//       std::cout << "sort random DAG " << (i + 1) << "/20" << std::endl;
//       // create a random DAG
//       char buffer[2048];
//       sprintf (buffer
//       , "Rscript scripts/createRandomDAG.R 100 0.5 %s %i"
//       , filename.c_str()
//       , i);
//       if (system ((char *) buffer)) 
//         std::cout << "something went wrong in the R script" << std::endl;
//       
//       std::vector <Edge> posEdges, negEdges;
//       readEdgesFromFile (filename, posEdges, negEdges);
//       
//       auto posDagAdjList = createGraphAdjListFromEdges (posEdges);
//       auto negDagAdjList = createGraphAdjListFromEdges (negEdges);
//       auto L = topologicalSortAdjList (posDagAdjList, negDagAdjList);
//       ASSERT_EQ (checkTopologicalSorting (L, posDagAdjList), true);
//       printVector <unsigned int> (L);
//     }
//   }
// }

// TEST (correctness, topologicalSortAdjList2) {
//   {
//     GraphAdjList posDag;
//     
//     auto L = topologicalSortAdjList2 (posDag, posDag);
//     ASSERT_EQ (checkTopologicalSorting (L, posDag), true);
//   }
//   
//   {
//     unsigned int nTests = 100;
//     srand (time(NULL));
//     
//     for (unsigned int i = 0; i < nTests; i++) {
//       std::cout << "sort random DAG " << (i + 1) << "/" << nTests << std::endl;
//       
//       auto filename = createRandomDAG (0.65, i);
//       std::vector <Edge> posEdges, negEdges;
//       readEdgesFromFile (filename, posEdges, negEdges);
//       
//       auto posDagAdjList = createGraphAdjListFromEdges (posEdges);
//       auto negDagAdjList = createGraphAdjListFromEdges (negEdges);
//       auto L = topologicalSortAdjList2 (posDagAdjList, negDagAdjList);
//       ASSERT_EQ (checkTopologicalSorting (L, posDagAdjList), true);
//       printVector <unsigned int> (L);
//     }
//   }
// }

TEST (correctness, topologicalSortAdjList3) {
  {
    GraphAdjList posDag;
    
    auto L = topologicalSortAdjList3 (posDag);
    ASSERT_EQ (checkTopologicalSorting (L, posDag), true);
  }
  
  {
    unsigned int nTests = 100;
    srand (time(NULL));
    
    for (unsigned int i = 0; i < nTests; i++) {
      std::cout << "sort random DAG " << (i + 1) << "/" << nTests << std::endl;
      
      auto filename = createRandomDAG (0.65, i);
      
      auto edges = readEdgesFromFile (filename);
      
      auto dagAdjList = createGraphAdjListFromEdges (edges);
      auto L = topologicalSortAdjList3 (dagAdjList);
      ASSERT_EQ (checkTopologicalSorting (L, dagAdjList), true);
//       printVector <unsigned int> (L);
    }
  }
}

TEST (correctness, topologicalSortCormanAdjList2) {
  {
    GraphAdjList posDag;
    
    auto L = topologicalSortCormanAdjList2 (posDag);
    ASSERT_EQ (checkTopologicalSorting (L, posDag), true);
  }
  
  {
    unsigned int nTests = 100;
    srand (time(NULL));
    
    for (unsigned int i = 0; i < nTests; i++) {
      std::cout << "sort random DAG " << (i + 1) << "/" << nTests << std::endl;
      
      auto filename = createRandomDAG (0.65, i);
      
      auto edges = readEdgesFromFile (filename);
      
      auto dagAdjList = createGraphAdjListFromEdges (edges);
      auto L = topologicalSortCormanAdjList2 (dagAdjList);
      ASSERT_EQ (checkTopologicalSorting (L, dagAdjList), true);
    }
  }
}

// TEST (correctness, topologicalSortCormanAdjList) {
//   {
//     GraphAdjList posDag;
//     
//     auto L = topologicalSortCormanAdjList (posDag);
//     ASSERT_EQ (checkTopologicalSorting (L, posDag), true);
//   }
//   
//   {
//     std::string filename = "/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/test-graph.dat";
//     
//     unsigned int nTests = 20;
//     
//     for (unsigned int i = 0; i < nTests; i++) {
//       std::cout << "sort random DAG " << (i + 1) << "/" << nTests << std::endl;
//       // create a random DAG
//       char buffer[2048];
//       sprintf (buffer
//       , "Rscript scripts/createRandomDAG-moreRandom.R 512 0.75 %s %i"
//       , filename.c_str()
//       , i + 1);
//       if (system ((char *) buffer)) 
//         std::cout << "something went wrong in the R script" << std::endl;
//       
//       auto edges = readEdgesFromFile (filename);
//       auto dagAdjList = createGraphAdjListFromEdges (edges);
//       auto L = topologicalSortCormanAdjList (dagAdjList);      
//       
//       printVector <unsigned int> (L);
//       
//       ASSERT_EQ (checkTopologicalSorting (L, dagAdjList), true);
//       
//     }
//   }
// }

// measure implementations of topological sorting
// TEST (measurements, topologicalSortAdjMatrix) { 
//   typedef std::function <std::vector <unsigned int> (Graph)> TopologicalSortFunctionHandle;
//   
//   std::vector <TopologicalSortFunctionHandle> topSortFunctions;
//   topSortFunctions.push_back (topologicalSort);
//   
//   std::vector <std::string> topSortFunctionNames;
//   topSortFunctionNames.push_back ("Kahn1962");
//   
//   
//   // configure benchmark
//   uint nrun = 3;
//   
//   uint minInputLength = 2;
//   uint maxInputLength = 4096; 
//   std::vector <uint> nNodesInDAG;
//   for (unsigned int n = minInputLength; n <= maxInputLength; n += n / 5 + 1) {
//     nNodesInDAG.push_back (n);
//   }
//   
// //   std::vector <uint> nNodesInDAG;
// //   fillWith2ndPower (1, 12, nNodesInDAG);
//   
//   std::vector <std::string> scenarioNames = {"sparse-graph", "dense-graph"};
//   // NOTE: here it is fixed, that 'int' values are sorted
//   std::vector <float> scenarios = {0.25, 0.9};
//   
//   // configure file-operations
//   const std::string resultDir = "/home/bach/Documents/algorithm-exercises/topological-sorting/measurements/";
//   
//   char temp [256];
//   std::sprintf (temp, "# %2s %10s %10s %10s %10s %15s\n", "n", "min[us]", "max[us]", "mean[us]", "sd[us]", "measurements[us]");
//   std::string headerTimes (temp);
//   
//   // run for every sorting algorithm
//   for (uint ind = 0; ind < topSortFunctions.size(); ind++) {
//     uint sceIndex = 0;
//     
//     std::printf ("topological sorting function %i/%lu\n", ind + 1, topSortFunctions.size());
//     
//     // run for every scenario
//     for (auto & sce : scenarios) {
//       
//       std::printf ("scenario %i/%lu\n", sceIndex + 1, scenarios.size());
//       
//       // create output file name
//       const std::string filename_time = resultDir + topSortFunctionNames[ind] + "-" + scenarioNames[sceIndex] + "-time";
//       if (isFileExisting (filename_time)) {
//         std::cout << "Warning:" << filename_time << " already exists. No measurements will be performed" << std::endl;
//         continue;
//       }
//       
//       // create matrices to store the measuring results
//       Matrix <timeDuration> timeDurationMeasurements (nNodesInDAG.size(), nrun, timeDuration());
//       uint i = 0;
//       
//       for (auto it = nNodesInDAG.begin(); it != nNodesInDAG.end(); ++it) {
//         // create a random DAG
//         char buffer[1024];
// //         sprintf (buffer, "Rscript scripts/createRandomDAG-moreRandom.R %i %.3f", *it, sce);
//         sprintf (buffer, "Rscript scripts/createRandomDAG.R %i %.3f", *it, sce);
//         if (system ((char *) buffer)) 
//           std::cout << "something went wrong in the R script" << std::endl;
//         
//         auto edges = readEdgesFromFile ("/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/benchmark-graph.dat");
//         auto dag = createGraphFromEdges (edges);
//         timeDurationMeasurements.setRow (i
//                                        , benchmark <timeDuration, timePoint, std::vector <unsigned int>, Graph> (myClock, nrun, topSortFunctions[ind], dag));
//         i++;
//       }
//       
//       // write out measurements
//       writeMeasurements <timeDuration> (filename_time, headerTimes, nNodesInDAG, timeDurationMeasurements);
//       
//       sceIndex++;
//     }
//   }
// }

// // measure implementations of topological sorting
// TEST (measurements, topologicalSortAdjList){ 
//   typedef std::function <std::vector <unsigned int> (GraphAdjList, GraphAdjList)> TopologicalSortFunctionHandle;
//   
//   std::vector <TopologicalSortFunctionHandle> topSortFunctions;
//   topSortFunctions.push_back (topologicalSortAdjList);
//   topSortFunctions.push_back (topologicalSortAdjList2);
//   
//   std::vector <std::string> topSortFunctionNames;
//   topSortFunctionNames.push_back ("Kahn1962-AdjList");
//   topSortFunctionNames.push_back ("Kahn1962-AdjList-2");
//   
//   
//   // configure benchmark
//   uint nrun = 3;
//   
//   uint minInputLength = 2;
//   uint maxInputLength = 4096; 
//   std::vector <uint> nNodesInDAG;
//   for (unsigned int n = minInputLength; n <= maxInputLength; n += n / 5 + 1) {
//     nNodesInDAG.push_back (n);
//   }
//   
//   //   std::vector <uint> nNodesInDAG;
//   //   fillWith2ndPower (1, 12, nNodesInDAG);
//   
//   std::vector <std::string> scenarioNames = {"sparse-graph", "dense-graph"};
//   // NOTE: here it is fixed, that 'int' values are sorted
//   std::vector <float> scenarios = {0.25, 0.9};
//   
//   // configure file-operations
//   const std::string resultDir = "/home/bach/Documents/algorithm-exercises/topological-sorting/measurements/";
//   
//   char temp [256];
//   std::sprintf (temp, "# %2s %10s %10s %10s %10s %15s\n", "n", "min[us]", "max[us]", "mean[us]", "sd[us]", "measurements[us]");
//   std::string headerTimes (temp);
//   
//   // run for every sorting algorithm
//   for (uint ind = 0; ind < topSortFunctions.size(); ind++) {
//     uint sceIndex = 0;
//     
//     std::printf ("topological sorting function %i/%lu\n", ind + 1, topSortFunctions.size());
//     
//     // run for every scenario
//     for (auto & sce : scenarios) {
//       
//       std::printf ("scenario %i/%lu\n", sceIndex + 1, scenarios.size());
//       
//       // create output file name
//       const std::string filename_time = resultDir + topSortFunctionNames[ind] + "-" + scenarioNames[sceIndex] + "-time";
//       if (isFileExisting (filename_time)) {
//         std::cout << "Warning:" << filename_time << " already exists. No measurements will be performed" << std::endl;
//         continue;
//       }
//       
//       // create matrices to store the measuring results
//       Matrix <timeDuration> timeDurationMeasurements (nNodesInDAG.size(), nrun, timeDuration());
//       uint i = 0;
//       
//       for (auto it = nNodesInDAG.begin(); it != nNodesInDAG.end(); ++it) {
//         // create a random DAG
//         char buffer[1024];
//         sprintf (buffer, "Rscript scripts/createRandomDAG.R %i %.3f", *it, sce);
//         if (system ((char *) buffer)) 
//           std::cout << "something went wrong in the R script" << std::endl;
//         
//         std::vector <Edge> posEdges, negEdges;
//         readEdgesFromFile ("/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/benchmark-graph.dat", posEdges, negEdges);
//         
//         auto posDag = createGraphAdjListFromEdges (posEdges);
//         auto negDag = createGraphAdjListFromEdges (negEdges);
//         timeDurationMeasurements.setRow (i
//         , benchmark <timeDuration, timePoint, std::vector <unsigned int>, GraphAdjList, GraphAdjList> (myClock, nrun, topSortFunctions[ind], posDag, negDag));
//         i++;
//       }
//       
//       // write out measurements
//       writeMeasurements <timeDuration> (filename_time, headerTimes, nNodesInDAG, timeDurationMeasurements);
//       
//       sceIndex++;
//     }
//   }
// }

// // measure implementations of topological sorting
// TEST (measurements, topologicalSortAdjList3){ 
//   typedef std::function <std::vector <unsigned int> (GraphAdjList)> TopologicalSortFunctionHandle;
//   
//   std::vector <TopologicalSortFunctionHandle> topSortFunctions;
//   topSortFunctions.push_back (topologicalSortAdjList3);
//   
//   std::vector <std::string> topSortFunctionNames;
//   topSortFunctionNames.push_back ("Kahn1962-AdjList-3");
//   
//   
//   // configure benchmark
//   uint nrun = 3;
//   
//   uint minInputLength = 2;
//   uint maxInputLength = 4096; 
//   std::vector <uint> nNodesInDAG;
//   for (unsigned int n = minInputLength; n <= maxInputLength; n += n / 5 + 1) {
//     nNodesInDAG.push_back (n);
//   }
//   
//   //   std::vector <uint> nNodesInDAG;
//   //   fillWith2ndPower (1, 12, nNodesInDAG);
//   
//   std::vector <std::string> scenarioNames = {"sparse-graph", "dense-graph"};
//   // NOTE: here it is fixed, that 'int' values are sorted
//   std::vector <float> scenarios = {0.25, 0.9};
//   
//   // configure file-operations
//   const std::string resultDir = "/home/bach/Documents/algorithm-exercises/topological-sorting/measurements/";
//   
//   char temp [256];
//   std::sprintf (temp, "# %2s %10s %10s %10s %10s %15s\n", "n", "min[us]", "max[us]", "mean[us]", "sd[us]", "measurements[us]");
//   std::string headerTimes (temp);
//   
//   // run for every sorting algorithm
//   for (uint ind = 0; ind < topSortFunctions.size(); ind++) {
//     uint sceIndex = 0;
//     
//     std::printf ("topological sorting function %i/%lu\n", ind + 1, topSortFunctions.size());
//     
//     // run for every scenario
//     for (auto & sce : scenarios) {
//       
//       std::printf ("scenario %i/%lu\n", sceIndex + 1, scenarios.size());
//       
//       // create output file name
//       const std::string filename_time = resultDir + topSortFunctionNames[ind] + "-" + scenarioNames[sceIndex] + "-time";
//       if (isFileExisting (filename_time)) {
//         std::cout << "Warning:" << filename_time << " already exists. No measurements will be performed" << std::endl;
//         continue;
//       }
//       
//       // create matrices to store the measuring results
//       Matrix <timeDuration> timeDurationMeasurements (nNodesInDAG.size(), nrun, timeDuration());
//       uint i = 0;
//       
//       for (auto it = nNodesInDAG.begin(); it != nNodesInDAG.end(); ++it) {
//         // create a random DAG
//         char buffer[1024];
//         sprintf (buffer, "Rscript scripts/createRandomDAG.R %i %.3f", *it, sce);
//         if (system ((char *) buffer)) 
//           std::cout << "something went wrong in the R script" << std::endl;
//         
//         auto edges = readEdgesFromFile ("/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/benchmark-graph.dat");
//         
//         auto dag = createGraphAdjListFromEdges (edges);
//         timeDurationMeasurements.setRow (i
//         , benchmark <timeDuration, timePoint, std::vector <unsigned int>, GraphAdjList> (myClock, nrun, topSortFunctions[ind], dag));
//         i++;
//       }
//       
//       // write out measurements
//       writeMeasurements <timeDuration> (filename_time, headerTimes, nNodesInDAG, timeDurationMeasurements);
//       
//       sceIndex++;
//     }
//   }
// }

// measure implementations of topological sorting
TEST (measurements, topologicalSortCormanAdjList){ 
  typedef std::function <std::vector <unsigned int> (GraphAdjList)> TopologicalSortFunctionHandle;
  
  std::vector <TopologicalSortFunctionHandle> topSortFunctions;
//   topSortFunctions.push_back (topologicalSortCormanAdjList);
  topSortFunctions.push_back (topologicalSortCormanAdjList2);
  topSortFunctions.push_back (topologicalSortAdjList3);
  
  
  std::vector <std::string> topSortFunctionNames;
//   topSortFunctionNames.push_back ("Corman-AdjList");
  topSortFunctionNames.push_back ("Corman-AdjList-2");
  topSortFunctionNames.push_back ("Kahn1962-AdjList-3");
  
  
  // configure benchmark
  uint nrun = 3;
  
  uint minInputLength = 2;
  uint maxInputLength = 2048; 
  std::vector <uint> nNodesInDAG;
  for (unsigned int n = minInputLength; n <= maxInputLength; n += n / 5 + 1) {
    nNodesInDAG.push_back (n);
  }
  
  //   std::vector <uint> nNodesInDAG;
  //   fillWith2ndPower (1, 12, nNodesInDAG);
  
  std::vector <std::string> scenarioNames = {"normal-graph"};
  // NOTE: here it is fixed, that 'int' values are sorted
  std::vector <float> scenarios = {0.65};
  
  // configure file-operations
  const std::string resultDir = "/home/bach/Documents/algorithm-exercises/topological-sorting/measurements/";
  
  char temp [256];
  std::sprintf (temp, "# %2s %10s %10s %10s %10s %15s\n", "n", "min[us]", "max[us]", "mean[us]", "sd[us]", "measurements[us]");
  std::string headerTimes (temp);
  
  // run for every sorting algorithm
  for (uint ind = 0; ind < topSortFunctions.size(); ind++) {
    uint sceIndex = 0;
    
    std::printf ("topological sorting function %i/%lu\n", ind + 1, topSortFunctions.size());
    
    // run for every scenario
    for (auto & sce : scenarios) {
      
      std::printf ("scenario %i/%lu\n", sceIndex + 1, scenarios.size());
      
      // create output file name
      const std::string filename_time = resultDir + topSortFunctionNames[ind] + "-" + scenarioNames[sceIndex] + "-time";
      if (isFileExisting (filename_time)) {
        std::cout << "Warning:" << filename_time << " already exists. No measurements will be performed" << std::endl;
        continue;
      }
      
      // create matrices to store the measuring results
      Matrix <timeDuration> timeDurationMeasurements (nNodesInDAG.size(), nrun, timeDuration());
      uint i = 0;
      
      for (auto it = nNodesInDAG.begin(); it != nNodesInDAG.end(); ++it) {
        // create a random DAG
        char buffer[1024];
        sprintf (buffer, "Rscript scripts/createRandomDAG-evenMoreRandom.R %i %.3f %s %i", *it, sce, "/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/benchmark-graph-fullrandom.dat", i);
        if (system ((char *) buffer)) 
          std::cout << "something went wrong in the R script" << std::endl;
        
        auto edges = readEdgesFromFile ("/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/benchmark-graph-fullrandom.dat");
        
        auto dag = createGraphAdjListFromEdges (edges);
        timeDurationMeasurements.setRow (i
        , benchmark <timeDuration, timePoint, std::vector <unsigned int>, GraphAdjList> (myClock, nrun, topSortFunctions[ind], dag));
        i++;
      }
      
      // write out measurements
      writeMeasurements <timeDuration> (filename_time, headerTimes, nNodesInDAG, timeDurationMeasurements);
      
      sceIndex++;
    }
  }
}

// profiling sorting functions
TEST (profiling, topologicalSorting) {
  std::string filename = "/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/test-graph.dat";
  
  std::vector <GraphAdjList> dags;
  
  unsigned int nDags = 1;
  
  for (unsigned int i = 0; i < nDags; i++) {
    std::cout << "sort random DAG " << (i + 1) << "/" << nDags << std::endl;
    // create a random DAG
    char buffer[2048];
    sprintf (buffer
    , "Rscript scripts/createRandomDAG-moreRandom.R 2048 0.75 %s %i"
    , filename.c_str()
    , i);
    if (system ((char *) buffer)) 
      std::cout << "something went wrong in the R script" << std::endl;
    
    dags.push_back(createGraphAdjListFromEdges (readEdgesFromFile (filename)));
  }
  
  for (auto & dag : dags) {
    topologicalSortCormanAdjList (dag);
  }
  
}

int main (int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}