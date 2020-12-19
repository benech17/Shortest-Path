#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../include/traces.h"

#include "adjacencyMatrix.h"
#include "treatmentFiles.h"

void courtChemie(T_graphMD * g, int * D, int * T, int * Pred, int sr);

int main(int argc, char ** argv) {

  CLRSCR();
	WHOAMI(); 

  T_graphMD * g = NULL;
  g = readFile("graph1.adj");

  createPNG("graph1", g, 2);

  int * D = (int *) malloc(g->nbVertices * sizeof(int));
  int * T = (int *) malloc(g->nbVertices * sizeof(int));
  int * Pred = (int *) malloc(g->nbVertices * sizeof(int));

  int i;
  for (i = 0; i < g->nbVertices; i++) {
    D[i] = 0;
    T[i] = 0;
    Pred[i] = 0;
  }

  showMatrix(g);

  courtChemie(g, D, T, Pred, 0);

  return 0;
}

void showVectors(int * D, int * T, int * Pred, int n) {

  int i;

  NL();
  printf("D: ");
  for (i = 0; i < n; i++) {
    if (D[i] != INT_MAX)
      printf("%d ", D[i]);
    else
      printf("inf ");
  }
  NL();

  printf("T: ");
  for (i = 0; i < n; i++) {
    printf("%d ", T[i]);
  }
  NL();

  printf("Pred: ");
  for (i = 0; i < n; i++) {
    printf("%d ", Pred[i]);
  }
  NL(); NL();

}

void compareIten(T_graphMD * g, int * D, int * Pred, int x, int y) {
  if (D[x] + g->mat[x][y] < D[y]) {
    D[y] = D[x] + g->mat[x][y];
    Pred[y] = x + 1;
  }
}

void courtChemie(T_graphMD * g, int * D, int * T, int * Pred, int sr) {

  int i;

  for (i = 0; i < g->nbVertices; i++) {
    D[i] = g->mat[sr][i];
    Pred[i] = (g->mat[sr][i] == INT_MAX) ? 0 : sr + 1;
  }

  T[sr] = 1;

  printf("\nEtape 0: ");
  showVectors(D, T, Pred, g->nbVertices);

  int stage = 1;

  while (1) {
    int k = -1;

    for (i = 0; i < g->nbVertices; i++) {
      if (T[i] == 0) {
        if (k ==-1)
          k = i;
        else {
          if (D[i] < D[k])
            k = i;
        }
      }
    }

    if (k == -1) {

      printf("Finish\n\n");
      return;
      
    }
      
    T[k] = 1;

    printf("Etape %d (avec %d):\n", stage, k + 1);

    for (i = 0; i < g->nbVertices; i++) {
      if (i != k && g->mat[k][i] < INT_MAX) {
        compareIten(g, D, Pred, k, i);
      }
    }

    stage++;
    showVectors(D, T, Pred, g->nbVertices);

    ENTER2CONTINUE();
    NL();
  }

}