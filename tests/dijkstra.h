
#include <time.h> 

#ifndef dijkstra_h
#define dijkstra_h

#define MAX_TESTS 10
#define MAX_majDijkstra 400

#define SHOW_RESULT_PROGRAM1
#define SHOW_RESULT_PROGRAM2

typedef struct {
  unsigned int nbVertices;
  int **mat;
}T_graphMD;

void dijkstra(T_graphMD *g, int *D, int *T, int *Pred, int sr);
void majDijkstra(int *F, int *D, int v, int k, int *indiceDansF);
T_graphMD * newGraphMD(int n);
void showProgramme1(int i);
void showProgramme2(int i);

void plotGraphic(clock_t timesMajDijkstra[]);
char * createProceduralMatrix(int length, T_graphMD *g);

#endif