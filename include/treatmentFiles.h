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

//programme1.c :

void dijkstra(T_graphMD*, int*, int*, int*, int, char*);
void showResult(int*, int*, int, int);
void majDijkstra(int*, int*, int, int, int*);
void compareItem(T_graphMD*, int*, int*, int, int);

#endif 
