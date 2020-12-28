#include "adjacencyMatrix.h"

#ifndef _READFILE_H_
#define _READFILE_H_

#define DOT_PATH

T_graphMD * newGraphMD(int n);
T_graphMD * readFile(char * filename);
void createPNG(char * filename, T_graphMD * g, int node, int * T, int stage);

#endif 
