param numColors;
param numNodes;
var z binary;

set edgeList dimen 2;

var color {i in 0..numNodes-1, c in 0..numColors-1} binary := 0;

param edge {i in 0..numNodes-1, j in 0..numNodes-1} binary :=
  if (i,j) in edgeList or (j,i) in edgeList then 1;
  
var conflict {i in 0..numNodes-1, j in 0..numNodes-1} binary;
	
minimize numConflicts: sum {i in 0..numNodes-1, j in 0..numNodes-1} conflict[i,j];

subject to c1 {i in 0..numNodes-1}:
	sum {c in 0..numColors-1} color[i,c] = 1;

	
c3 {i in 0..numNodes-1, j in 0..numNodes-1, c in 0..numColors-1}:
	(if edge[i,j] = 1 && color[i,c] = 1 && color[j,c] = 1 
	then conflict[i,j] = 1;

c5 {i in 0..numNodes-1, j in 0..numNodes-1}:
	edge[i,j] = 0 ==> conflict[i,j] = 0;