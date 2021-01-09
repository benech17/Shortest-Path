#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TESTS 13

char * createProceduralMatrix(int length);

int main(int argc, char ** argv) {

  char * filename;
  int i, j;

  for (i = 4; i < MAX_TESTS; i++) {
    filename = createProceduralMatrix(i);

    printf("\nexécution du programme 1 avec %d nodes\n", i);

    char command[200];

    for (j = 0; j < i; j++) {

      sprintf(command, "rm -r -f output/dotFiles/randGraph%d_%d", i, j);
      system(command);

      sprintf(command, "rm -r -f output/pngFiles/randGraph%d_%d", i, j);
      system(command);

      sprintf(command, "./../programme1/programme1.exe %s %d -all", filename, j);
    
      system(command);

    }

    if (i + 1 < MAX_TESTS)
      printf("------------------------------------------- \n");
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
    }

  }
  
  char * filename = (char *) malloc(100 * sizeof(char));
  sprintf(filename, "input/randGraph%d.adj", length);
  
  createAdjFile(filename, length, matrix);

  return filename;

}