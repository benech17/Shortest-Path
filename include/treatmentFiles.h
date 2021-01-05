#ifndef _READFILE_H_
#define _READFILE_H_

#define DOT_PATH

typedef struct {
  unsigned int nbVertices;
  int **mat;
}T_graphMD;

T_graphMD * newGraphMD(int);
T_graphMD * readFile(char *);
void createPNG(char *, T_graphMD *,int, int *,int);

#endif 
