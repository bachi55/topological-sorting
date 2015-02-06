# Provides access to a copy of the command line arguments supplied when 
# this R session was invoked.
args <- commandArgs(trailingOnly = TRUE)

## args: read input arguments
# [1]: input file-name 

nArgin <- length (args)
if (nArgin < 1) {
  stop ("Not enough input arguments given.")
  quit (status=1, save="no")
}

filename <- args[1]

read.table (filename, header=FALSE, sep=" ")

require (igraph)