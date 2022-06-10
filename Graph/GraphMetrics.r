#!/usr/bin/env Rscript

library(igraph)
library(plyr)

# Read arguments
args <- commandArgs(trailingOnly = TRUE)

# LOAD DATA
file <- args[ 1 ]
best <- as.double(args[ 2 ])

trace <- read.table(file, sep= "", header=TRUE, colClasses=c("integer", "integer",
                                            "character", "integer","character"))
colnames(trace) <- c("run","fit1","node1","fit2","node2")

# rbind combine by rows
lnodes <- rbind( setNames(trace[c("node1","fit1")], c("Node","Fitness")),
                 setNames(trace[c("node2","fit2")], c("Node","Fitness")))
ledges <- trace[c("node1","node2")]

# ddply delete duplicand nodes or edges
nodes <- ddply( lnodes, .(Node,Fitness), nrow )
colnames(nodes) <- c("Node","Fitness","Count")
edges <- ddply( ledges, .(node1,node2), nrow )
colnames(edges) <- c("Start","End","Count")

LON <- graph_from_data_frame( d=edges, directed=T, vertices=nodes)              # CREATE THE GRAPH
LON <- simplify(LON, remove.multiple=FALSE, remove.loops=TRUE)                  # REMOVE SELF LOOPS


############################# COMPUTE PERFORMANCE METRICS

# Success rate
nrun <- max(trace$run)
success <- 0
for (i in 1 : nrun ) {
  if( length( which( trace$run == i & trace$fit2 == best ) ) >= 1 )
    success <- success + 1;
}
success <- success/nrun

# Deviation --------------- ( DEVIATION FROM THE END OF THE RUN TO THE GLOBAL BEST )
dev <- c(rep(0, nrun))
for (i in 1 : nrun ) {
  dev[i] <- abs( min( trace$fit2[which( trace$run == i)] ) - best )
}
deviation <- mean( dev )
sddeviation <- sd( dev )

############################# COMPUTE THE LON METRICS

# Number of optima (local and global)
noptima <- vcount(LON)

# Funnels
sinks_id <- which( degree( LON, mode="out" ) == 0 )
nfunnels <- length( sinks_id )
sinks_fit <- vertex_attr(LON, "Fitness")[sinks_id]
global_opt <- which( sinks_fit == best )
ngfunnels <- length( global_opt )

# Neutral
el <- as_edgelist( LON )
fits <- V(LON)$Fitness
names <- V(LON)$name
f1 <- fits[match(el[,1], names)]
f2 <- fits[match(el[,2], names)]
gnn <- subgraph.edges(LON, which(f2==f1), delete.vertices=TRUE)     # Network with the same fitness value
gnn <- simplify(gnn, remove.multiple=FALSE, remove.loops = TRUE)
neutral <- round(vcount(gnn)/noptima,4)

# Strength   # Number of input links to the node
igs <- which( V(LON)$Fitness == best )
stren_igs <- sum( strength(graph = LON, vids = igs, mode = "in", loops = F), na.rm = TRUE ) # Dont considerer missing data
stren_all <- sum( strength(graph = LON, mode = "in", loops = F), na.rm = TRUE )
if ( stren_all > 0.0 ) {
  strength <- stren_igs / stren_all
  strength <- round(strength,4)
} else {
  strength <- 0.0
}

# Display metrics
cat( paste0("noptima: ", noptima, "\n"))
cat( paste0("nfunnels: ", nfunnels, "\n" ))
cat( paste0("ngfunnels: ", ngfunnels, "\n" ))
cat( paste0("neutral: ", neutral, "\n" ))
cat( paste0("strength: ", strength, "\n" ))
cat( paste0("success: ", success, "\n" ))
cat( paste0("deviation: ", deviation, "\n" ))
cat( paste0("sd_deviation: ", sddeviation, "\n" ))

############################# COMPUTE THE CMLON METRICS

el <- as_edgelist( LON )
fits <- V(LON)$Fitness
names <- V(LON)$name
f1 <- fits[match(el[,1], names)]
f2 <- fits[match(el[,2], names)]
E(LON)[which(f2<f1)]$type <- "improving"   # improving edges - Minimisation
E(LON)[which(f2==f1)]$type <- "equal"      # equal fitness (neutral) edges
MLON <- LON
V(MLON)$Count<-1
gnn <- subgraph.edges(LON, which(f2==f1), delete.vertices=FALSE)     # Network with the same fitness value
nn_memb <- components(gnn, mode="weak")$membership
CMLON <- contract.vertices(MLON, mapping = nn_memb, vertex.attr.comb = list(Fitness = "first", Count = "sum", "ignore"))
CMLON <- simplify(CMLON, edge.attr.comb = list(Count="sum"))


# FUNNELS
sinks_id <-which(degree(CMLON, mode = "out")==0)
sinks_fit <- vertex_attr(CMLON, "Fitness")[sinks_id]
global_opt <- which( sinks_fit == best )
nfunnels <- length(sinks_id)     # Total Number of funnels
ngfunnels <- length(global_opt)     # Number of global funnels

# NEUTRAL
neutral = round(1.0-vcount(CMLON)/vcount(LON),4)

# STRENGTH
igs<-sinks_id[sinks_fit==best]  # index of global sinks
ils<-sinks_id[sinks_fit>best]   # index of local sinks -- might be empty (minimisation problem)
sing <- sum(strength(graph = CMLON, vids = igs, mode = "in", loops = F), na.rm = T)  # global sinks
sinl <- sum(strength(graph = CMLON, vids = ils, mode = "in", loops = F), na.rm = T)  # local sinks
if ( (sing+sinl) > 0.0 ) {
  strength <- sing / ( sing + sinl )
  strength <- round(strength,4)
} else {
  strength <- 0.0
}

# Display metrics
cat( paste0("noptima: ", vcount(CMLON), "\n" ))
cat( paste0("nfunnels: ", nfunnels, "\n" ))
cat( paste0("ngfunnels: ", ngfunnels, "\n" ))
cat( paste0("neutral: ", neutral, "\n" ))
cat( paste0("strength: ", strength, "\n" ))
