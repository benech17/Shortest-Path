#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/treatmentFiles.h"

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

void createPNG(char * filename, T_graphMD * g, int node, int * T, int stage, int sr) {
  
  FILE * filePNG = NULL;
  
  char number[100];

  int i, j; 

  char fileNameComplet[150] = "";

  sprintf(fileNameComplet, "output/dotFiles/%s", filename);

  mkdir(fileNameComplet, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  sprintf(fileNameComplet, "output/dotFiles/%s/%s_%d", filename, filename, sr);

  mkdir(fileNameComplet, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  sprintf(fileNameComplet, "output/dotFiles/%s/%s_%d/%s_%d.dot", filename, filename, sr, filename, stage);
  
  filePNG = fopen(fileNameComplet, "w+");

  putHeader(filePNG);

  for (i = 0; i < g->nbVertices; i++) {
    number[0] = '\0';
    sprintf(number, "%d", i);

    fputs(number, filePNG);

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

  for (i = 0; i < g->nbVertices; i++) {

    for (j = 0; j < g->nbVertices; j++) {
      if (g->mat[i][j] != INT_MAX && i != j) {
        sprintf(number, "\t%d -> %d [label = \"%d\"];\n", i, j, g->mat[i][j]);
        fputs(number, filePNG);
      }
    }
  }

  fputs("}", filePNG);

  fclose(filePNG);

  char directoryPNG[150] = "";

  sprintf(directoryPNG, "output/pngFiles/%s", filename);
  mkdir(directoryPNG, 0777);
  
  sprintf(directoryPNG, "output/pngFiles/%s/%s_%d", filename, filename, sr);
  mkdir(directoryPNG, 0777);

  char comand[350] = "";
  sprintf(comand, "dot %s -T png -o output/pngFiles/%s/%s_%d/%s_%d.png", fileNameComplet, filename, filename, sr, filename, stage);

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

  T_graphMD * g = NULL;

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
    printf("\n\t%s\n\tfichier non trouvé, veuillez essayer un autre nom\n\n", filename);
  }

  return g;
}