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
  clock_t timesMajDijkstra[MAX_majDijkstra]; //Vecteur qui stocke toutes les temps collectées

  for (i = 4; i < MAX_TESTS; i++) {

    //Définition des variables
    T_graphMD *g = NULL;
    g = newGraphMD(i);
    int D[i], T[i], Pred[i];

    //Appelle la fonction qui crée un fichier .adj et renvoie son nom
    filename = createProceduralMatrix(i, g);
    
    CLRSCR();

    printf("\nexécution du programme 1 avec %d sommets\n", i);

    char command[200];

    //Pour chaque matrice aléatoire créée, tous les points seront testés comme point d'origine
    for (j = 0; j < i; j++) {

      //Comme il peut y avoir des fichiers .dot et .png comme vestiges d'autres tests, 
      //la meilleure chose à faire est de les nettoyer d'abord
      sprintf(command, "rm -r -f output/dotFiles/randGraph%d_%d", i, j);
      system(command);

      sprintf(command, "rm -r -f output/pngFiles/randGraph%d_%d", i, j);
      system(command);

      //Exécute le programme 1, en passant la matrice aléatoire comme paramètre et lui demande d'afficher tous les chemins
      sprintf(command, "./../programme1/programme1.exe %s %d -all", filename, j);
      system(command);

      //pour éviter la pollution de l'écran, 
      //si SHOW_RESULT_PROGRAM1 est défini, l'écran sera nettoyé
      showProgramme1(i);

    }

    //Les lignes de code suivantes visent à exécuter le programme 2 de trois manières : 
    //convertir adj en la, puis convertir la en adj, 
    //et enfin convertir adj en dot/png

    printf("--------------------------------------------------\n");
    printf("\nexécution du programme 2 avec %d sommets\n\n", i);

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

    //comme dans ce morceau de code, certaines matrices d'adj. ont déjà été développées,
    //trouver le temps de trouver le meilleur chemin
    //pour cela, le temps final est soustrait au temps initial
    timesMajDijkstra[i - 4] = clock();
    dijkstra(g, D, T, Pred, 0);
    timesMajDijkstra[i - 4] = clock() - timesMajDijkstra[i - 4];

    //est vidée à varivale g
    //les autres variables (D, T, Pred) sont automatiquement vidées à chaque boucle
    free(g);

    ENTER2CONTINUE();

  }

  //Dans cette partie du code, le souci est de faire un test avec plus d'éléments.
  //Comme pour les lignes précédentes, les matrices sont créées, puis le temps d'exécution de la fonction dijkstra est calculé
  for (i=i; i < MAX_majDijkstra + 4; i++) {
    T_graphMD *g = NULL;
    g = newGraphMD(i);
    int D[i], T[i], Pred[i];

    filename = createProceduralMatrix(i, g);

    //Comme de nombreux fichiers d'adjectifs indésirables seraient créés, un nettoyage est fait dans ces
    char command[100];
    sprintf(command, "rm -r %s", filename);
    system(command);

    timesMajDijkstra[i - 4] = clock();
    dijkstra(g, D, T, Pred, 0);
    timesMajDijkstra[i - 4] = clock() - timesMajDijkstra[i - 4];

    free(g);
  }

  //enfin, la fonction qui va créer le graphique est appelée
  plotGraphic(timesMajDijkstra);

  return 0;
}


void plotGraphic(clock_t timesMajDijkstra[]){

  //Le principe de cette fonction est de créer deux fichiers : 
  //un fichier .dat qui contient toutes les valeurs de temps, 
  //et un fichier .plt qui contient les commandes pour la création du graphique
  
  //ce modèle a été repris par le matériel mis à disposition en classe, dans la quatrième semaine
  //pour cette procédure, la fonction "gnuplot" a été utilisée
  //Référence: http://www.gnuplot.info/
  FILE * fp;
  int i;

  mkdir("output/graphic", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  fp = (FILE * )fopen("output/graphic/dijkstra.dat", "w+");

  //d'abord, le fichier de données est créé, et rempli avec une taille (sommets - 4) 
  //et la durée de la fonction djikstra
  if (fp != NULL)
  {
    fprintf(fp, "#+--------+----------------+\n");
    fprintf(fp, "#| Taille | Durée (ms)  |\n");
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
    printf("\nL'archive de données n'a pas pu être créé\n\n");
  }

  fp = (FILE * )fopen("output/graphic/dijkstra.plt", "w+");

  //dans l'ordre est créé le fichier contenant les lignes pour créer le graphique
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
    printf("\nL'archive de plot n'a pas pu être créé\n\n");
  }
  
  //enfin, le code qui va générer le graphique à partir des données 
  //et du fichier contenant les propriétés du graphique est exécuté
  char command[100];
  sprintf(command,"%s %s 2>/dev/null\n", "gnuplot", "output/graphic/dijkstra.plt");
  system(command);

  printf("\nLe graphique a été créé en output/graphic/dijkstra.png\n\n");
}

//cette fonction vise à créer le fichier contenant la matrice adjacente, 
//à partir d'une matrice préalablement développée
void createAdjFile(char * filename, int length, int matrix[length][length]) {

  int i, j;
  FILE * fileAdj = NULL;

  fileAdj = fopen(filename, "w+");

  //L'idée est essentiellement de passer par tous les points de la matrice, 
  //et d'écrire dans le fichier, en faisant attention aux valeurs infinies
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

//Cette fonction a deux objectifs, le premier est de créer la matrice aléatoire de manière procédurale, 
//et de remplir la structure g afin de l'utiliser ultérieurement
char * createProceduralMatrix(int length, T_graphMD *g) {

  g->nbVertices = length;
  int matrix[length][length];
  int i, j;

  //pour éviter une dépendance à une fonction aléatoire, cette fonction est exécutée
  srand(time(NULL));

  //cette première boucle est liée à la colonne de la matrice (logique inversée)
  for (i = 0; i < length; i++) {

    //est stipulé quand les éléments auront par colonne, avec au moins deux valeurs, 
    //donc il y a un moyen d'aller dans n'importe quel sommet
    int quantityPerColum = 2 + rand() % (length / 4);
    
    //cette deuxième boucle est en relation avec les lignes
    for (j = 0; j < length; j++){
      if (i != j) {
        //l'idée des deux lignes suivantes est de développer une formule qui 
        //randomisera les éléments d'une colonne qui auront des valeurs d'infinité différentes, 
        //en prenant le montant total par colonne 
        double probility = (quantityPerColum * 100.0) / (length - j); //porcentage
        
        int differentFromZero = (rand() % 100 <= probility) ? 1 : 0;

        //enfin, la valeur est placée dans la matrice, 
        //en tirant une nouvelle fois au sort sa valeur
        if (differentFromZero == 1) {
          quantityPerColum--;
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
  
  //enfin, le nom du fichier qui sera créé par la fonction createAdjFile est créé
  char * filename = (char *) malloc(100 * sizeof(char));
  sprintf(filename, "input/randGraph%d.adj", length);
  
  createAdjFile(filename, length, matrix);

  //est renvoyé le nom du fichier créé
  return filename;

}