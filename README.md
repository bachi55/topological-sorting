Topological Sorting
===================

Project for the course: Algorithm Engineering in the winter-term 14/15

Description:
  Aim of the project was to implement, test and measure algorithm for topological sorting. 

Algorithms:
  The following two algorithms have been implemented (see wikipedia for pseudo-code: http://en.wikipedia.org/wiki/Topological_sorting):
    (1) Kahn1962
    (2) Corman et al. 
 
Evaluation:
  1) Adjacency-matrix vs Adjacency-list
  -------------------------------------
  Both algorithms have been successive improved. First a adjacency matrix was chosen to represent the graph. It turned out, that for this representation some needed operations does "cost a lot". Therefore in a second step a adjacency list has been implemented. Both representations have been evaluated first on the Kahn1962 algorithm. Benchmarks revealed, that using a adjacency list, does not perform better than the algorithm using matrices. See [measurements/plot1a and plot1b] for details. Note that in the plot also the "naive" Corman implementation is given, which served as an reference. 
  
  Sparse graphs does contain just a small amount of edges and dense ones are close to complete.
  
  2) Profiling of Adjacency-list
  ------------------------------
  Profiling revealed that the adjacency-list does have an expensive operation namely "eraseEdge". This operations performs in O(|E|) and in the Kahn1962 algorithm it is called very often. Equipped with that knowledge some improvements of Kahn1962 algorithm has been done. 
    (1) for the set of zero-degree nodes a stack is used
    (2) only the in-degree of every node is stored and decreased by one if an "parent" node has been deleted
  The improvements manifest in the implementation 3 ('topologicalSortingAdjList3'). See [measurements/plot2a and plot2b]. It can be seen, that now the Kahn1962 algorithm does perform as well as the Corman et al. does. 
  
  3) Corman et al. vs Kahn1962
  ----------------------------
  See [measurements/plot3a and plot3b] for a comparison between the fastest Kahn1962 and Corman et al. implementations. Both implementations are very close and the Kahn1962 can be slightly better. But with some small optimizations for Corman et al. (less memory usage and pre-allocated memory, 'topologicalSortingCormanAdjList2') it is better than the Kahn1962 one. 
  
  4) Smaller input
  ----------------
  On smaller input does the Kahn1962 algorithm ('topologicalSortingAdjList3') perform best. See [measurements/plot4a and plot4b].
  
  5) Influence of the "random DAG"
  --------------------------------
  To create random DAG the [scripts/createRandomDAG.R] has been used. It exploits a special property of an adjacency matrix of DAG. Every triangular matrix is an adjacency matrix of a DAG. Therefore it is straightforward to create random lower-triangular matrices and export them as adjacency lists, which can be read from the C++ code. 
  
  However, matrices created like this have special structure. Maybe a "more random" adjacency-matrix would be better. Therefore another [scripts/createRandomDAG-evenMoreRandom.R] has been implemented, which is creating random DAG by adding edges randomly and thereby keeping the DAG property. 
  
  [measurements/plot5a and plot5b] show the time measurements for the best sorting algorithms for the different random-DAG-scripts. It can be seen that ...
  
