#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../include/traces.h"

#include "adjacencyMatrix.h"
#include "treatmentFiles.h"

void courtChemie(T_graphMD * g, int * D, int * T, int * Pred, int sr,  char * filename);
void showResult(int * D, int * Pred, int src, int dst);
char * correctFile(char * filename);

int main(int argc, char ** argv) {

  if (argc != 4){
    printf("\n\t3 paramètres doivent être saisis\n\n");
    return 0;
  }

  int sr = atoi(argv[2]);
  int dst = atoi(argv[3]);

  T_graphMD * g = NULL;

  char * filename = correctFile(argv[1]);

  g = readFile(filename);

  int * D = (int *) malloc(g->nbVertices * sizeof(int));
  int * T = (int *) malloc(g->nbVertices * sizeof(int));
  int * Pred = (int *) malloc(g->nbVertices * sizeof(int));

  int i;
  for (i = 0; i < g->nbVertices; i++) {
    D[i] = 0;
    T[i] = 0;
    Pred[i] = 0;
  }

  char * name = strtok(filename, ".");

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

  int i;

  for (i = 0; i < g->nbVertices; i++) {
    D[i] = g->mat[sr][i];
    Pred[i] = (g->mat[sr][i] == INT_MAX) ? 0 : sr;
  }

  T[sr] = 1;

  createPNG(filename, g, 0, T, 0);

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

      createPNG(filename, g, stage, T, stage);
      return;
      
    }
      
    createPNG(filename, g, k, T, stage);
    T[k] = 1;

    for (i = 0; i < g->nbVertices; i++) {
      if (i != k && g->mat[k][i] < INT_MAX) {
        compareItem(g, D, Pred, k, i);
      }
    }

    stage++;
  }

}

char * correctFile(char * filename) {
  char * filenameCorrect = (char *) malloc(sizeof(char) * (11 + strlen(filename)));
  filenameCorrect[0] = 'p';
  filenameCorrect[1] = 'r';
  filenameCorrect[2] = 'o';
  filenameCorrect[3] = 'g';
  filenameCorrect[4] = 'r';
  filenameCorrect[5] = 'a';
  filenameCorrect[6] = 'm';
  filenameCorrect[7] = 'm';
  filenameCorrect[8] = 'e';
  filenameCorrect[9] = '1';

  int l;
  for (l = 1; filename[l] != '\0'; l++) {
    filenameCorrect[l + 9] = filename[l];
  }
  filenameCorrect[l + 9] = '\0';

  return filenameCorrect;
}