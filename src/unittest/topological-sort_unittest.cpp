#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <vector>

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

TEST (correctness, readGraphFromFile) {
  auto edges = readEdgesFromFile ("example-graphs/test-graph.dat");
  
  auto graphAdjMatrix = createGraphFromEdges (edges);
//   graphAdjMatrix.printMatrix();
  
  auto graphAdjList = createGraphAdjListFromEdges (edges);
//   graphAdjList.printGraph();
}

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

TEST (correctness, topologicalSort) {
  // NOTE: There is topological-sort implementation in linux, which could
  //       serve as reference. Check: http://en.wikipedia.org/wiki/Tsort
  //       and 'tsort' in the terminal.
  
  {
    Graph dag;
    
    auto L = topologicalSort (dag);
    ASSERT_EQ (checkTopologicalSorting (L, dag), true);
  }
  
  {
    Graph dag (2, 2, {
      0, 1,
      0, 0
    });
    
    auto L = topologicalSort (dag);
    ASSERT_EQ (checkTopologicalSorting (L, dag), true);
  }
  
  {
    Graph dag (2, 2, {
      0, 0,
      1, 0
    });
    
    auto L = topologicalSort (dag);
    ASSERT_EQ (checkTopologicalSorting (L, dag), true);
  }
  
  {
    Graph dag (3, 3, {
      0, 1, 1,
      0, 0, 1,
      0, 0, 0
    });
    
    auto L = topologicalSort (dag);
    ASSERT_EQ (checkTopologicalSorting (L, dag), true);
  }
  
  {
    // NOTE: This graph comes from: http://en.wikipedia.org/wiki/Topological_sorting
    Graph dag (11, 11, {
  //  1  2  3  4  5  6  7  8  9 10 11
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1 * 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2 * 
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, // 3
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4 * 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, // 5
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6 *
      0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, // 7
      0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, // 8 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9 *
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10 *
      0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0  // 11
    });
    
    auto L = topologicalSort (dag);
    ASSERT_EQ (checkTopologicalSorting (L, dag), true);
  }
  
  {
    Graph dag (4, 4, {
      0, 1, 1, 1,
      0, 0, 1, 0,
      0, 0, 0, 1,
      0, 0, 0, 0
    });
    
    auto L = topologicalSort (dag);
    ASSERT_EQ (checkTopologicalSorting (L, dag), true);
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
  
}

typedef std::function <std::vector <size_t> (Graph)> TopologicalSortFunctionHandle;

TEST (measurements, topologicalSort) { 
  
  std::vector <TopologicalSortFunctionHandle> topSortFunctions;
  topSortFunctions.push_back (topologicalSort);
  
  std::vector <std::string> topSortFunctionNames;
  topSortFunctionNames.push_back ("Kahn1962");
  
  
  // configure benchmark
  uint nrun = 3;
  uint minInputLength = 10;
  uint maxInputLength = 2500; 
  std::vector <uint> nNodesInDAG;
  for (size_t n = minInputLength; n <= maxInputLength; n += n / 10 + 1)
    nNodesInDAG.push_back (n);
  
  std::vector <std::string> scenarioNames = {"sparse-graph", "normal-graph", "dense-graph"};
  // NOTE: here it is fixed, that 'int' values are sorted
  std::vector <float> scenarios = {0.25, 0.5, 0.75};
  
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
      
      // create matrices to store the measuring results
      Matrix <timeDuration> timeDurationMeasurements (nNodesInDAG.size(), nrun, timeDuration());
      uint i = 0;
      
      for (auto it = nNodesInDAG.begin(); it != nNodesInDAG.end(); ++it) {
        // create a random DAG
        char buffer[1024];
        sprintf (buffer, "Rscript scripts/createRandomDAG.R %i %.3f", *it, sce);
        if (system ((char *) buffer)) 
          std::cout << "something went wrong in the R script" << std::endl;
        
        auto edges = readEdgesFromFile ("/home/bach/Documents/algorithm-exercises/topological-sorting/example-graphs/benchmark-graph.dat");
        auto dag = createGraphFromEdges (edges);
        timeDurationMeasurements.setRow (i
                                       , benchmark <timeDuration, timePoint, std::vector <size_t>, Graph> (myClock, nrun, topSortFunctions[ind], dag));
        i++;
      }
      
      // write out measurements
      const std::string filename_time = resultDir + topSortFunctionNames[ind] + "-" + scenarioNames[sceIndex] + "-time";
      writeMeasurements <timeDuration> (filename_time, headerTimes, nNodesInDAG, timeDurationMeasurements);
      
      sceIndex++;
    }
  }
}

int main (int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}