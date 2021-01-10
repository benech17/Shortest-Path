#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/treatmentFiles.h"

//est fonction écrit essentiellement dans le fichier de votre paramètre, 
//le code qui représente un code pour un certain sommet.
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

//cette fonction met les codes initiaux du fichieito .dot
void putHeader(FILE * filePNG){
  fputs("digraph graphe {\n", filePNG);
  fputs("rankdir = LR;\n", filePNG);
  fputs("edge [color=red];\n", filePNG);
}

//si DOT_PATH est défini, la fonction createPNG créera l'image, 
//sinon elle ne fera rien
#ifdef DOT_PATH

void createPNG(char * filename, T_graphMD * g, int node, int * T, int stage, int sr) {
  
  FILE * filePNG = NULL;
  
  char number[100];

  int i, j; 

  char fileNameComplet[150] = "";

  //d'abord, les dossiers sont créés

  mkdir("output/dotFiles", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  mkdir("output/pngFiles", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  sprintf(fileNameComplet, "output/dotFiles/%s", filename);

  mkdir(fileNameComplet, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  sprintf(fileNameComplet, "output/dotFiles/%s/%s_%d", filename, filename, sr);

  mkdir(fileNameComplet, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  //dans l'ordre, le fichier .dot est créé et ouvert
  sprintf(fileNameComplet, "output/dotFiles/%s/%s_%d/%s_%d.dot", filename, filename, sr, filename, stage);
  
  filePNG = fopen(fileNameComplet, "w+");

  //l'en-tête par défaut du fichier est placé.
  putHeader(filePNG);

  //pour chaque ligne de la matrice adjacente
  for (i = 0; i < g->nbVertices; i++) {

    //pour chaque colonne de la matrice adjacente 
    for (j = 0; j < g->nbVertices; j++) {

      //est vérifiée s'il y a une valeur et si elle est différente de 0 ou infinie, 
      //elle est donc placée dans le .dot
      if (g->mat[i][j] != INT_MAX && i != j) {
        sprintf(number, "\t%d -> %d [label = \"%d\"];\n", i, j, g->mat[i][j]);
        fputs(number, filePNG);
      }
    }
  }

  //dans cette boucle, l'idée est de passer par chaque sommet est de mettre sa couleur
  //cette couleur est d'abord définie par le sommet qui a été choisi par la fonction Dijkstra (vert),
  //alors si le sommet a été choisi auparavant (rouge)
  //sinon c'est bleu

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


  fputs("}", filePNG);

  fclose(filePNG);

  //les lignes de code suivantes créent les répertoires pngs, 
  //ainsi que l'exécution de la commande qui convertit le code .dot en .png
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

//fonction qui crée la structure T_graphMD, et crée ses valeurs
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

//cette fonction vise à lire un fichier .dot 
//et à renvoyer la structure T_graphMD obtenue par cette lecture
T_graphMD * readFile(char * filename) {

  T_graphMD * g;

  FILE * fileMatrix;

  //d'abord, une tentative d'ouverture du dossier
  fileMatrix = fopen(filename, "r");

  if (fileMatrix != NULL) {

    char val;
    char number[11];
    int i, j, k;
    i = j = k = 0;

    //Cette première lecture du fichier se vérifie jusqu'à la fin de 
    //la première ligne, donc obtenir la taille de la matrice
    while (fscanf(fileMatrix, "%c", &val) != EOF){
      if (val == '\t')
        i++;
      else if (val == '\n') {
        i++;
        break;
      }
    }

    //la structure est calculée en fonction de cette taille
    //ainsi que la lecture du dossier est reprise au début
    g = newGraphMD(i);
    i = 0;
    fseek(fileMatrix, 0, SEEK_SET);

    //Dans cette section, le code passe par tous les caractères du fichier, 
    //et à chaque numéro trouvé, il est placé dans la structure, chaque espacement 
    //avance d'un indice de la colonne, et à chaque changement de ligne, avance d'un indice de la ligne.
    //Si elle trouve i, la matrice de la structure est remplie par la valeur maximale de l'entier
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
    
    //fermer le dossier
    fclose(fileMatrix);

  }
  else {
    printf("error\n\n");
  }

  //retour la structure
  return g;
}