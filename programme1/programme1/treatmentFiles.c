#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/traces.h"
#include "adjacencyMatrix.h"
#include "treatmentFiles.h"

void putColorNode(FILE * filePNG, char color) {

  fputs(" [fontname=\"Arial\", shape = circle, ", filePNG);
  if (color == 'b')
    fputs("color=lightblue", filePNG);
  else if (color == 'g')
    fputs("color=green", filePNG);
  else
    fputs("color=red", filePNG);
  fputs(", style=filled];\n", filePNG);

}

void putHeader(FILE * filePNG){
  fputs("digraph graphe {\n", filePNG);
  fputs("rankdir = LR;\n", filePNG);
  fputs("edge [color=red];\n", filePNG);
}

#ifdef DOT_PATH

void createPNG(char * filename, T_graphMD * g, int node, int * T) {
  
  FILE * filePNG = NULL;

  char fileNameComplet[150] = "";
  strcat(fileNameComplet, "programme1/output/dotFiles/");
  strcat(fileNameComplet, filename);

  mkdir(fileNameComplet, 0777);

  strcat(fileNameComplet, "/");
  strcat(fileNameComplet, filename);

  strcat(fileNameComplet, "_");

  char auxNumber[11] = "";
  sprintf(auxNumber, "%d", node);

  strcat(fileNameComplet, auxNumber);

  strcat(fileNameComplet, ".dot");
  
  filePNG = fopen(fileNameComplet, "w+");

  putHeader(filePNG);

  int i, j; 

  for (i = 0; i < g->nbVertices; i++) {
    auxNumber[0] = '\0';
    sprintf(auxNumber, "%d", i);

    fputs(auxNumber, filePNG);
    if (i == node) {
      putColorNode(filePNG, 'g');
    }
    else if (T[i] == 1) {
      putColorNode(filePNG, 'r');
    }
    else{
      putColorNode(filePNG, 'b');
    }
  }
  
  char number[26];

  for (i = 0; i < g->nbVertices; i++) {

    for (j = 0; j < g->nbVertices; j++) {
      if (g->mat[i][j] != INT_MAX && i != j) {
        
        fputs("\t", filePNG);

        sprintf(number, "%d %s %d", i, " -> ", j);
        fputs(number, filePNG);

        fputs(" [label = \"", filePNG);

        sprintf(number, "%d", g->mat[i][j]);
        fputs(number, filePNG);

        fputs("\"];\n", filePNG);
      }
    }
  }

  fputs("}", filePNG);
  system("");

  fclose(filePNG);

  char comand[350] = "dot ";
  strcat(comand, fileNameComplet);
  strcat(comand, " -T png -o programme1/output/pngFiles/");

  char directoryPNG[150] = "programme1/output/pngFiles/";
  strcat(directoryPNG, filename);
  mkdir(directoryPNG, 0777);

  strcat(comand, filename);
  strcat(comand, "/");
  strcat(comand, filename);
  strcat(comand, "_");

  auxNumber[0] = '\0';
  sprintf(auxNumber, "%d", node);

  strcat(comand, auxNumber);
  strcat(comand, ".png");

  system(comand);

}

#else

void createPNG(char * filename, T_graphMD * g, int node, int * T) {

}

#endif

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
      else if (val == '\n') {
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
    

    fclose(fileMatrix);

  }
  else {
    printf("erro\n\n");
  }


  return g;
}


