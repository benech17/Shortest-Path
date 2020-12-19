#include "adjacencyMatrix.h"

#ifndef _READFILE_H_
#define _READFILE_H_

T_graphMD * newGraphMD(int n);
T_graphMD * readFile(char * filename);
void showMatrix (T_graphMD * g);
void createPNG(char * filename, T_graphMD * g, int trace);

#endif 
