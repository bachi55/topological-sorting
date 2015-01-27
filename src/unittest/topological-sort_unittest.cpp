#include <gtest/gtest.h>

#include <iostream>

#include "topological-sort.h"

// function to print a vector
template <typename T>
void printVector (const std::vector <T> & v, const std::string & prefix = "") {
  std::cout << prefix;
  for (auto it = v.begin(); it != v.end(); ++it)
    std::cout << (*it) << " ";
  std::cout << std::endl;
};

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
  
  // TODO: Read graphs from file for example
  
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

int main (int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}