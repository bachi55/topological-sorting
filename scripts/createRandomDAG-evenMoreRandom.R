require (igraph)

# Provides access to a copy of the command line arguments supplied when 
# this R session was invoked.
args <- commandArgs(trailingOnly = TRUE)

## args: read input arguments
# [1]: amount of nodes within the graph
# [2]: probability of having an edge between two nodes of the DAG
# [3]: output file-name 
# [4]: random seed

nArgin <- length (args)
if (nArgin < 1) {
  stop ("Not enough input arguments given.")
  quit (status=1, save="no")
}

# amount of nodes within the graph
  nNodes        <- as.numeric (args[1])
# probability of having an edge between two nodes of the DAG
if (nArgin > 1) { 
  epsilon       <- as.numeric (args[2])
} else {
  epsilon       <- 0.5
}

# output-filename
if (nArgin > 2) {
  filename      <- args[3]
} else {
  filename      <- filename <- "~/Documents/algorithm-exercises/topological-sorting/example-graphs/benchmark-graph-fullrandom.dat"
}

# random seed
if (nArgin > 3) {
  set.seed         (as.numeric (args[4]))
}

## create the DAG adjacency matrix
# create an empty adjacency matrix (nNodes x nNodes)
# NOTE: adjacency matrix is logical (FALSE='no edge between a and b', TRUE = !FALSE)
graph <- matrix (0, nrow=nNodes, ncol=nNodes)
# get logical matrix indexing the lower triangle matrix of 'graph'
g <- graph.adjacency (graph)

upperLimitOfAmountEdges <- ((nNodes**2) - nNodes) / 2
nEdges <- 0

print (nNodes)
pb <- txtProgressBar (min = 0, max = round (epsilon * upperLimitOfAmountEdges), style = 3)

while (nEdges < round (epsilon * ((nNodes**2) - nNodes) / 2)) {
  # get random edges between two nodes
  edge <- round (runif (2, min=1, max=nNodes))
  g[edge[1], edge[2]] <- 1
  
  # check whether the dag-property is still valid
  if (is.dag (g)) {
    nEdges <- nEdges + 1
  } else {
    g[edge[1], edge[2]] <- 0
  }
    
  # update progress bar
  setTxtProgressBar(pb, nEdges)  
}

close (pb)


graph <- get.adjacency (g, sparse=FALSE)
nEdges <- sum (graph)

rm (list=c("g"))

## transform DAG adjacency matrix into a list of adjacent nodes
# e.g. (1) --> (2) would be represented by the "1 2"-line in the adjacency list file

# TODO: save memory by writing line by line into the file
adjacencyList <- matrix (0, nEdges, 2)
i <- 1

for (sourceNode in 1:nNodes) {
  # get a list of the target nodes reacheable from the current node
  targetNodes <- which (graph[sourceNode, ] == TRUE)
  # write all the directed edges into a file
  # NOTE: indexing should start with zero
  for (targetNode in targetNodes) {
    adjacencyList[i, ] <- c(sourceNode - 1, targetNode - 1)
    i <- i + 1
  }
  # TODO: How to handle if the node with the highest id is a node with no incomming and outgoing edges?
  # if a node exists, which does not have any outgoing-edge it will be stored with as: (id) --> (-1)
#   if (length (targetNodes) == 0) {
#     print("bla")
# #     adjacencyList[i, ] <- c(sourceNode - 1, -1)
# #     i <- i + 1
#   }
}

# write list to a file
if (! file.create (filename)) {
  stop ("Output file could not be created.")
  quit (status=1, save="no")
}
write.table (adjacencyList
           , file=filename
           , row.names=FALSE
           , col.names=FALSE)
           
## if every thing went fine, tell it to the shell :)           
quit (status=0, save="no")


