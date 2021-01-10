#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <time.h> 
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/traces.h"
#include "dijkstra.h"

int main(int argc, char ** argv) {

  char * filename;
  int i, j;
  clock_t timesMajDijkstra[MAX_majDijkstra];

  for (i = 4; i < MAX_TESTS; i++) {

    T_graphMD *g = NULL;
    g = newGraphMD(i);
    int D[i], T[i], Pred[i];

    filename = createProceduralMatrix(i, g);
    
    CLRSCR();

    printf("\nexécution du programme 1 avec %d nodes\n", i);

    char command[200];

    for (j = 0; j < i; j++) {

      sprintf(command, "rm -r -f output/dotFiles/randGraph%d_%d", i, j);
      system(command);

      sprintf(command, "rm -r -f output/pngFiles/randGraph%d_%d", i, j);
      system(command);

      sprintf(command, "./../programme1/programme1.exe %s %d -all", filename, j);
      system(command);

      showProgramme1(i);

    }

    printf("--------------------------------------------------\n");
    printf("\nexécution du programme 2 avec %d nodes\n\n", i);

    printf("\tcommand: ./../programme2/programme2.exe -la -a %s\n", filename);
    sprintf(command, "./../programme2/programme2.exe -la -a %s", filename);
    NL(); 
    system(command);
    showProgramme2(i);
    NL();
    
    printf("\tcommand: ./../programme2/programme2.exe -adj -l output/la/Graph%d.la\n", i);
    sprintf(command, "./../programme2/programme2.exe -adj -l output/la/Graph%d.la", i);
    NL(); 
    system(command);
    showProgramme2(i);
    NL(); 
    
    printf("\tcommand: ./../programme2/programme2.exe -dot -a output/adj/Graph%d.adj\n", i);
    sprintf(command, "./../programme2/programme2.exe -dot -a output/adj/Graph%d.adj\n", i);
    NL(); 
    system(command);
    showProgramme2(i);
    NL(); 

    timesMajDijkstra[i - 4] = clock();
    dijkstra(g, D, T, Pred, 0);
    timesMajDijkstra[i - 4] = clock() - timesMajDijkstra[i - 4];

    free(g);

    ENTER2CONTINUE();

  }

  for (i=i; i < MAX_majDijkstra + 4; i++) {
    T_graphMD *g = NULL;
    g = newGraphMD(i);
    int D[i], T[i], Pred[i];

    filename = createProceduralMatrix(i, g);
    char command[100];
    sprintf(command, "rm -r %s", filename);
    system(command);

    timesMajDijkstra[i - 4] = clock();
    dijkstra(g, D, T, Pred, 0);
    timesMajDijkstra[i - 4] = clock() - timesMajDijkstra[i - 4];

    free(g);
  }

  plotGraphic(timesMajDijkstra);

  return 0;
}


void plotGraphic(clock_t timesMajDijkstra[]){

  FILE * fp;
  int i;

  mkdir("output/graphic", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  fp = (FILE * )fopen("output/graphic/dijkstra.dat", "w+");

  if (fp != NULL)
  {
    fprintf(fp, "#+--------+----------------+\n");
    fprintf(fp, "#| Taille | Durátion (ms)  |\n");
    fprintf(fp, "#+--------+----------------+\n");

    for (i = 0; i < MAX_majDijkstra; i++)
    {
      fprintf(fp,"%8d   %6lu    \n", i, (unsigned long)
        (timesMajDijkstra[i] * 1000 * 1000)/(CLOCKS_PER_SEC));
      
    }

    fclose(fp);
  }
  else 
  {
    printf("\nle archive de données n'a pas pu être créé\n\n");
  }

  fp = (FILE * )fopen("output/graphic/dijkstra.plt", "w+");

  if (fp != NULL)
  {
    fprintf(fp, "#+-------------------------------+\n");
    fprintf(fp, "#| Fichier de commandes gnuplot  |\n");
    fprintf(fp, "#+-------------------------------+\n");

    fprintf(fp, "set style data lines\n");
    fprintf(fp, "set xlabel \"Nombre d'éléments\"\n");
    fprintf(fp, "set ylabel \"Durátion (ms)\"\n");
    fprintf(fp, "set title \"Dijkstra\"\n");
    fprintf(fp, "set grid\n");
    fprintf(fp, "set terminal png\n");
    fprintf(fp, "set output 'output/graphic/dijkstra.png'\n");
    fprintf(fp, "plot 'output/graphic/dijkstra.dat' using 1:2 title 'aleatoire'\n");
    fprintf(fp, "exit\n");

    fclose(fp);
  }
  else
  {
    printf("\nle archive de plot n'a pas pu être créé\n\n");
  }
  
  char command[100];
  sprintf(command,"%s %s 2>/dev/null\n", "gnuplot", "output/graphic/dijkstra.plt");
  system(command);

  printf("\nLe graphique a été créé en output/graphic/dijkstra.png\n\n");
}

void createAdjFile(char * filename, int length, int matrix[length][length]) {

  int i, j;
  FILE * fileAdj = NULL;

  fileAdj = fopen(filename, "w+");

  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++) {
      char number[5];

      if (j == 0 && matrix[i][j] == 99)
        sprintf(number, "i");
      else if (matrix[i][j] == 99)
        sprintf(number, "\ti");
      else if (j == 0) 
        sprintf(number, "%d", matrix[i][j] );
      else
        sprintf(number, "\t%d", matrix[i][j]);
      
      fputs(number, fileAdj);
    }

    fputs("\n", fileAdj);
  }

  fclose(fileAdj);

}

char * createProceduralMatrix(int length, T_graphMD *g) {

  g->nbVertices = length;
  int matrix[length][length];
  int i, j;

  srand(time(NULL));

  for (i = 0; i < length; i++) {
    int quantityPerLine = 2 + rand() % (length / 4);
    
    for (j = 0; j < length; j++){
      if (i != j) {
        double probility = (quantityPerLine * 100.0) / (length - j); //porcentage
        
        int differentFromZero = (rand() % 100 <= probility) ? 1 : 0;

        if (differentFromZero == 1) {
          quantityPerLine--;
          matrix[j][i] = (4 + rand() % 20);
        }
        else 
          matrix[j][i] = 99;

      }
      else 
        matrix[i][i] = 0;
        
      g->mat[j][i] = (matrix[j][i] == 99) ? INT_MAX : matrix[j][i];
    }

  }
  
  char * filename = (char *) malloc(100 * sizeof(char));
  sprintf(filename, "input/randGraph%d.adj", length);
  
  createAdjFile(filename, length, matrix);

  return filename;

}