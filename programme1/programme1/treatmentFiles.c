#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../include/traces.h"
#include "adjacencyMatrix.h"
#include "treatmentFiles.h"

void putHeader(FILE * filePNG){
  fputs("digraph graphe {\n", filePNG);
  fputs("rankdir = LR;\n", filePNG);
  fputs("node [fontname=\"Arial\", shape = circle, color=lightblue, style=filled];\n", filePNG);
  fputs("edge [color=red];\n", filePNG);
}

void createPNG(char * filename, T_graphMD * g, int trace) {
  
  FILE * filePNG;

  filePNG = fopen(filename, "w+");
  putHeader(filePNG);

  int i, j; 
  char number[26];

  for (i = 0; i < g->nbVertices; i++) {
    for (j = 0; j < g->nbVertices; j++) {
      if (g->mat[i][j] != INT_MAX && i != j) {
        fputs("\t", filePNG);

        sprintf(number, "%d %s %d", i + 1, " -> ", j + 1);
        fputs(number, filePNG);

        fputs(" [label = \"", filePNG);

        sprintf(number, "%d", g->mat[i][j]);
        fputs(number, filePNG);

        fputs("\"];\n", filePNG);
      }
    }
  }

  fputs("}", filePNG);

  fclose(filePNG);

}

T_graphMD * newGraphMD(int n) {
  T_graphMD * g;
  int i, j;

  g = (T_graphMD *) malloc( sizeof(T_graphMD) );

  g->nbVertices = n;
  g->mat = (int **)malloc(n * sizeof(int *));

  for (i = 0; i < n; i++) 
    g->mat[i] = (int *)malloc(n * sizeof(int));

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      g->mat[i][j] = INT_MAX;
    }
  }
  return g;
}

T_graphMD * readFile(char * filename) {

  T_graphMD * g;

  FILE * fileMatrix;
  fileMatrix = fopen(filename, "r");

  if (fileMatrix != NULL) {

    char val;
    char number[11];
    int i, j, k;
    i = j = k = 0;

    while (fscanf(fileMatrix, "%c", &val) != EOF){
      if (val == '\t')
        i++;
      else if (val == '\n') {
        i++;
        break;
      }
    }

    printf("matrice: %d x %d\n\n", i, i);

    g = newGraphMD(i);
    i = 0;
    fseek(fileMatrix, 0, SEEK_SET);

    while (fscanf(fileMatrix, "%c", &val) != EOF) {
      if (val == '\t') {
        number[k] = '\0';
        if (number[0] != 'i') 
          g->mat[i][j] = atoi(number);

        k = 0;
        j++;
      }
      if (val == '\n') {
        number[k] = '\0';
        if (number[0] != 'i') 
          g->mat[i][j] = atoi(number);

        k = 0;
        j = 0;
        i++;
      }
      else if ((val >= '0' && val <= '9') || (val >= 'a' && val <= 'z')) {
        number[k] = val;
        k++;
      }
    }

  }
  else {
    printf("erro\n\n");
  }

  fclose(fileMatrix);

  return g;
}

void showMatrix (T_graphMD * g) {
  int i, j;
  for (i = 0; i < g->nbVertices; i++) {
    for (j = 0; j < g->nbVertices; j++) {
      if (g->mat[i][j] != INT_MAX)
        printf("\t%d", g->mat[i][j]);
      else
        printf("\tin");
      
      if (j < g->nbVertices - 1)
        printf("\t|");
    }
    NL();
  }
  NL();
}


