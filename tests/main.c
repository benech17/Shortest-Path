#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char * createProceduralMatrix(int length);

int main(int argc, char ** argv) {

  char * filename;
  int i;

  for (i = 4; i < 15; i++) {
    filename = createProceduralMatrix(i);

    printf("exécution du programme 1 avec %d nodes\n", i);
    printf("meilleur chemin de 0 à %d\n", (i - 1));

    char command[200];
    sprintf(command, "./../programme1/programme1.exe %s 0 %d", filename, (i - 1));
  
    system(command);
  }

  return 0;
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

char * createProceduralMatrix(int length) {

  int matrix[length][length];
  int i, j;

  srand(time(NULL));

  for (i = 0; i < length; i++) {
    int quantityPerLine = 2 + rand() % (length / 4);

    if (length <= 3)
      quantityPerLine = length - 1;
    
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
        matrix[i][i] = 99;
    }

  }
  
  char * filename = (char *) malloc(100 * sizeof(char));
  sprintf(filename, "input/randGraph%d.adj", length);
  
  createAdjFile(filename, length, matrix);

  return filename;

}