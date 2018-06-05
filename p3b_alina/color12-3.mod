param numColors;
param numNodes;

set edgeList dimen 2;

param edge {i in 0..numNodes-1, j in 0..numNodes-1} binary :=
  if (i,j) in edgeList or (j,i) in edgeList then 1;
