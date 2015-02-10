require (ggplot2)

## compile filenames
scenarios <- c("dense-graph", "sparse-graph")
for (sce in scenarios) {

  # plot1a/b
  algorithms <- c("Corman-AdjList", "Kahn1962", "Kahn1962-AdjList")
  # plot2a/b
  algorithms <- c("Corman-AdjList", "Kahn1962", "Kahn1962-AdjList", "Kahn1962-AdjList-3")
  # plot3a/b
  algorithms <- c("Corman-AdjList", "Kahn1962-AdjList-3", "Corman-AdjList-2")
  # plot4a/b
  algorithms <- c("Corman-AdjList-2", "Kahn1962-AdjList-3", "Kahn1962")
  filenames <- paste ("~/Documents/algorithm-exercises/topological-sorting/measurements/"
      , paste (algorithms)
      , sep=""
  )
  tmpStr <- paste (sce, "-time", sep="")
#   tmpStr <- paste (sce, "time", sep="")
  filenames.Times <- paste (filenames, tmpStr, sep="-")
#   tmpStr <- paste (sce, "-cycles", sep="")
#   tmpStr <- paste (sce, "cycles", sep="")
#   filenames.Cycles <- paste (filenames, tmpStr, sep="-")

  nAlgorithms <- length (algorithms)

  algorithmsNames <- algorithms
#   algorithmsNames <- c("std::sort", "mergesort (naive)", "mergesort (inplace merge)", "quicksort (naive)", "quicksort (+ all)")
#   algorithmsNames <- c("std::sort", "naive", "inplace merge", "std::inplace_merge")

  ## extract data to plot (times)
  x.space <- 10:200
#   x.space <- 1:10000
  nMeasurements <- max (x.space) - min (x.space) + 1

  mean <- vector()
  max <- vector()
  min <- vector()

  for (file in filenames.Times) {
    print (file)
    tmp <- read.table (file, header=F, comment.char="", skip=1) 
    mean <- c(mean, c(tmp[, 5]))
    min <- c(min, c(tmp[, 3]))
    max <- c(max, c(tmp[, 4]))
  }
  
  x.min <- 0
  x.max <- 64
  x.space.selection <- which ((tmp[, 2] >= x.min) & (tmp[, 2] <= x.max))
  
  x.space <- tmp[x.space.selection, 2]
#   nMeasurements <- nrow (tmp)
  nMeasurements <- length (x.space.selection)

  cbPalette <- c("#000000", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")

  df <- data.frame (cbind (rep(x.space, nAlgorithms), min[(tmp[, 2] >= x.min) & (tmp[, 2] <= x.max)], max[(tmp[, 2] >= x.min) & (tmp[, 2] <= x.max)], mean[(tmp[, 2] >= x.min) & (tmp[, 2] <= x.max)]))
  
  df[, 5] <- gl (nAlgorithms, nMeasurements, nAlgorithms * nMeasurements, labels=algorithmsNames)
  colnames (df) <- c("n", "min", "max", "mean", "algorithm")

  ggplot (df, aes (x=n, y=mean)) +
    ggtitle (sce) +	
#     ggtitle (paste ("Quicksort (", sce, ")", sep="")) + 
    ylim(min (min[(tmp[, 2] >= x.min) & (tmp[, 2] <= x.max)]), max (max[(tmp[, 2] >= x.min) & (tmp[, 2] <= x.max)])) + xlab ("n") + ylab ("times (us)") +
    geom_line (aes (group=algorithm, color=algorithm), alpha=0.75, size=0.3) +
    geom_ribbon (aes (ymin=min, ymax=max, color=algorithm, fill=algorithm), alpha=0.25, size=0.3) +
    scale_color_manual(values=cbPalette) + 
    scale_fill_manual(values=cbPalette)

  ggsave (paste ("~/Documents/algorithm-exercises/topological-sorting/measurements/plot-time-", sce, ".pdf", sep=""), width=8, height=4)  
    
  # dev.off()  

  ## extract data to plot (cycles)
#   mean <- vector()
#   max <- vector()
#   min <- vector()
# 
#   for (file in filenames.Cycles) {
#     tmp <- read.table (file, header=F, comment.char="", skip=1) 
#     mean <- c(mean, c(tmp[, 5], rep (0, nMeasurements - nrow (tmp))))
#     min <- c(min, c(tmp[, 3], rep (0, nMeasurements - nrow (tmp))))
#     max <- c(max, c(tmp[, 4], rep (0, nMeasurements - nrow (tmp))))
#   }
#   cbPalette <- c("#000000", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")
# 
#   df <- data.frame (cbind (rep(x.space, nAlgorithms), min, max, mean))
#   df[, 5] <- gl (nAlgorithms, nMeasurements, nAlgorithms * nMeasurements, labels=algorithmsNames)
#   colnames (df) <- c("n", "min", "max", "mean", "algorithm")
# 
#   ggplot (df, aes (x=n, y=mean)) +
#     ggtitle (sce) +
#     ylim(0, 4000000) + xlab ("n") + ylab ("cylces (#)") +
#     geom_line (aes (group=algorithm, color=algorithm)) +
# #     geom_ribbon (aes (ymin=min, ymax=max, color=algorithm, fill=algorithm), alpha=0.5) +
#     scale_color_manual(values=cbPalette) + 
# #     scale_fill_manual(values=cbPalette)  
#     
#   ggsave (paste ("~/Documents/algorithm-exercises/sort/measurements/plot-cycles-", sce, ".pdf", sep=""), width=8, height=4)    
  
}