#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../include/traces.h"

#include "adjacencyMatrix.h"
#include "treatmentFiles.h"

void courtChemie(T_graphMD * g, int * D, int * T, int * Pred, int sr,  char * filename);
void showResult(int * D, int * Pred, int src, int dst);
void majDijkstra(int * F, int * D, int v, int k, int * indiceDansF);

int main(int argc, char ** argv) {

  if (argc != 4){
    printf("\n\t3 paramètres doivent être saisis\n\n");
    return 0;
  }

  int sr = atoi(argv[2]);
  int dst = atoi(argv[3]);

  T_graphMD * g = NULL;
  g = readFile(argv[1]);


  int * D = (int *) malloc(g->nbVertices * sizeof(int));
  int * T = (int *) malloc(g->nbVertices * sizeof(int));
  int * Pred = (int *) malloc(g->nbVertices * sizeof(int));

  int i;
  for (i = 0; i < g->nbVertices; i++) {
    D[i] = 0;
    T[i] = 0;
    Pred[i] = 0;
  }

  char * name = strtok(argv[1], ".");

  for (i = 11; name[i] != '\0'; i++)
    name[i - 11] = name[i];
    
  name [i - 11] = '\0';
  

  courtChemie(g, D, T, Pred, sr, name);

  if (dst < g->nbVertices) {
    printf("\n%d\n", D[dst]);
    showResult(D, Pred, sr, dst);
    NL();
  }
  else 
    printf("This is impossible");

  NL();

  return 0;
}

void showResult(int * D, int * Pred, int src, int dst) {

  if (dst != src) {
    showResult(D, Pred, src, Pred[dst]);
    printf (" -> %d", dst);
  }
  else 
    printf("%d", src);
  
}

void compareItem(T_graphMD * g, int * D, int * Pred, int x, int y) {
  if (D[x] + g->mat[x][y] < D[y]) {
    D[y] = D[x] + g->mat[x][y];
    Pred[y] = x;
  }
}

void courtChemie(T_graphMD * g, int * D, int * T, int * Pred, int sr, char * filename) {
  int u;
  int v;
  int i = 1;
  int k = 0;
  int F[g->nbVertices];
  int indiceDansF[g->nbVertices];

  for(u=0;u<g->nbVertices;u++) {
    Pred[u] = 0;
    if(u == sr) {
      D[u] = 0;
      F[0] = u;
      indiceDansF[u] = 0;
    }
    else {
      D[u] = INT_MAX;
      F[i] = u;
      indiceDansF[u] = i;
      i++;
    }
  }

  while(k<g->nbVertices) {
    u = F[k];
    k++;
    for(v=0;v<g->nbVertices;v++) {
      if ((v!=u) & (g->mat[u][v] != INT_MAX)) {
        if (D[v] > D[u] + g->mat[u][v]) {

          D[v] = D[u] + g->mat[u][v];
          Pred[v] = u;
          majDijkstra(F,D,v,k,indiceDansF);

        }
      }
    }
  }

}

void majDijkstra(int * F, int * D, int v, int k, int * indiceDansF) {
  int i = indiceDansF[v]-k;
  int c;
  while (((i/2) >= 1) && (D[F[(i/2)+k]] > D[F[i+k]])) {

    c = F[i+k];
    F[i+k] = F[(i/2)+k];
    F[(i/2)+k] = c;

    indiceDansF[F[i+k]] = i+k;
    indiceDansF[F[(i/2)+k]] = (i/2)+k;

    i = i/2;
  }
}