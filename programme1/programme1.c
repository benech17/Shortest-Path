#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "../include/treatmentFiles.h"

 int isNumber(char *token)
  {
    for (int i = 0; i < strlen(token); i++)
    {
      if (!isdigit(token[i]))
      {
       printf("");
        return 0;
      }
    }
    return 1;
  }
int main(int argc, char **argv)
{

  int sr, dst;
  char *modifier = "\0";

  //d'abord, le nombre de paramètres est vérifié
  if (argc != 4)
  {
    printf("\n\t3 paramètres doivent être saisis\n\n");
    return 0;
  }

  //les sommets source et destination sont attribués
  if (isNumber(argv[2]) == 1 && isNumber(argv[3]) == 1)
  {
    sr = atoi(argv[2]);
    dst = atoi(argv[3]);
  }
  else
  {
    printf("\n\tveuillez entrer des numéros valides\n\n");
    exit(-1);
  }

  //si au lieu d'une destination, le paramètre -all a été défini,
  //il n'y a pas de nœud de destination
  if (strcmp(argv[3], "-all") == 0)
  {
    dst = 1;
    modifier = argv[3];
  }

  //A ce moment, le code appelle la fonction
  //readFile qui lira le fichier .adj passé par paramètre
  T_graphMD *g = NULL;
  g = readFile(argv[1]);

  //s'il n'existe pas, le code est finalisé
  if (g == NULL)
    return 0;

  //Les vecteurs D, T, Pred sont initialisés
  int *D = (int *)malloc(g->nbVertices * sizeof(int));
  int *T = (int *)malloc(g->nbVertices * sizeof(int));
  int *Pred = (int *)malloc(g->nbVertices * sizeof(int));

  //Dans un premier temps, le vecteur T est remis à zéro
  int i;
  for (i = 0; i < g->nbVertices; i++)
  {
    T[i] = 0;
  }

  //puisque l'extension du fichier n'est pas importante,
  //la fonction strtok coupera la String dès qu'elle trouvera le caractère '.'
  char *filename = strtok(argv[1], ".");

  //l'idée des lignes suivantes est de supprimer le texte initial "input/" de la String
  for (i = 6; filename[i] != '\0'; i++)
    filename[i - 6] = filename[i];

  filename[i - 6] = '\0';

  //À ce stade, le code appelle la fonction principale qui trouvera les meilleurs
  //chemins vers tous les sommets à partir d'une source
  dijkstra(g, D, T, Pred, sr, filename);

  //si le paramètre a été défini, tous les chemins résultants seront affichés
  if (strcmp(modifier, "-all") == 0)
  {
    printf("\nTous les chemins à partir de %d\n", sr);

    //dans cette boucle, la fonction showResult sera appelée pour chaque sommet de la matrice
    for (i = 0; i < g->nbVertices; i++)
    {
      printf("(%d) ", D[i]);
      showResult(D, Pred, sr, i);
      printf("\n");
    }

    printf("\n");
  }
  else
  {
    //dans ce cas, la fonction showResult ne sera appelée qu'une seule fois
    if (dst < g->nbVertices)
    {
      printf("\n%d\n", D[dst]);
      showResult(D, Pred, sr, dst);
      printf("\n\n");
    }
    else
    {
      //si la destination n'est pas un sommet de matrice
      printf("Ce chemin est impossible.\n\n");
    }
  }

  //tous les vecteurs auxiliaires sont vidés
  free(D);
  free(T);
  free(Pred);
  free(g);
  return 0;
}

void showResult(int *D, int *Pred, int sr, int dst)
{

  if (dst != sr)
  {
    if (Pred[dst] == -1)
    {                                                             //Si la valeur reste celle de l'initialisation
      printf("Le chemin n'existe pas (%d -> .. -> %d)", sr, dst); //Il n'y a pas de chemin
    }
    else
    {
      showResult(D, Pred, sr, Pred[dst]);
      printf(" -> %d", dst);
    }
  }
  else
    printf("%d", sr);
}

void dijkstra(T_graphMD *g, int *D, int *T, int *Pred, int sr, char *filename)
{

  //Modifie les tableaux D et Pred pour obtenir le plus court chemin entre sr et les différents
  //sommets du graphe g en appliquant l'algorithme de Dijkstra

  int u;
  int v;
  int i = 1;                      //Indice indiquant le nombre de sommet initialisé (hors source)
  int k = 0;                      //Indice indiquant le nombre de sommet traité
  int F[g->nbVertices];           //Création de la file de priorité
  int indiceDansF[g->nbVertices]; //Tableau tel que F[indiceDansF[u]] = u

  for (u = 0; u < g->nbVertices; u++) //Initialisation de D et Pred
  {
    Pred[u] = -1; //Valeur négative pour ne pas confondre avec un sommet existant
    if (u == sr)
    {
      D[u] = 0;
      F[0] = u;
      indiceDansF[u] = 0;
    }
    else
    {
      D[u] = INT_MAX;
      F[i] = u;
      indiceDansF[u] = i;
      i++;
    }
  }

  while (k < g->nbVertices)
  {
    u = F[k];            //On récupère le premier sommet de la file de priorité (on a déjà traité les k premiers)
    if (D[u] != INT_MAX) //Si le sommet a bien été atteint lors des itérations précédentes
    {
      T[u] = 1;
      createPNG(filename, g, u, T, k, sr); //Création du PNG pour cette étape
    }
    k++; //Incrémentation du nombre de sommet traité
    if (D[u] != INT_MAX)
    {
      for (v = 0; v < g->nbVertices; v++) //Pour chaque sommet
      {
        if ((v != u) && (g->mat[u][v] != INT_MAX) && (D[v] > D[u] + g->mat[u][v]))
        { //S'il est voisin du sommet courant et que le chemin par le sommet courant est plus court

          D[v] = D[u] + g->mat[u][v]; //On obtient un nouveau chemin
          Pred[v] = u;
          majDijkstra(F, D, v, k, indiceDansF); //On s'assure que la file de priorité est à jour
        }
      }
    }
  }

  createPNG(filename, g, -1, T, k, sr); //Création du PNG final

  // create a video of successive generated PNG files
  char fileNameComplet[150] = "";
  sprintf(fileNameComplet, "output/pngFiles/%s/%s_%d/%s", filename, filename, sr, filename);

  char command[400] = "";
  sprintf(command, "ffmpeg -r 1 -s 1920x1080 -i %s_%%1d.png -vcodec libx264 -crf 25 -y -loglevel quiet  %s.mp4", fileNameComplet, fileNameComplet);

  system(command);
}

void majDijkstra(int *F, int *D, int v, int k, int *indiceDansF)
{
  //Met à jour la file de priorité F lorsqu'une nouvelle distance plus courte pour atteindre le
  //sommet v a été trouvée

  int i = indiceDansF[v] - k;                                 //On récupère l'indice de v dans F, et on prend en compte le décalage
  int c;                                                      //dû aux sommets déjà traités
  while (((i / 2) >= 1) && (D[F[(i / 2) + k]] > D[F[i + k]])) //Tant que le sommet d'indice i/2 est à
  {                                                           //une distance de sr plus importante

    c = F[i + k];
    F[i + k] = F[(i / 2) + k]; //On échange les deux sommets dans la file
    F[(i / 2) + k] = c;

    indiceDansF[F[i + k]] = i + k; //On remet à jour le tableaux des indices
    indiceDansF[F[(i / 2) + k]] = (i / 2) + k;

    i = i / 2;
  }
  if (D[F[(i / 2) + k]] > D[F[i + k]]) //Dernière itération pour le cas où le premier élément
  {                                    //a une distance à sr plus importante

    c = F[i + k];
    F[i + k] = F[(i / 2) + k];
    F[(i / 2) + k] = c;

    indiceDansF[F[i + k]] = i + k;
    indiceDansF[F[(i / 2) + k]] = (i / 2) + k;
  }

 
}
