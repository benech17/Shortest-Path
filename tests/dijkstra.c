#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#include "dijkstra.h"
#include "../include/traces.h"

//Cette fonction est la même que pour le code 1

void dijkstra(T_graphMD *g, int *D, int *T, int *Pred, int sr)
{
  int u;
  int v;
  int i = 1;
  int k = 0;
  int F[g->nbVertices];
  int indiceDansF[g->nbVertices];

  for (u = 0; u < g->nbVertices; u++)
  {
    Pred[u] = -1;
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
    u = F[k];
    if (D[u] != INT_MAX)
    {
      T[u] = 1;
    }
    k++;
    if (D[u] != INT_MAX)
    {
      for (v = 0; v < g->nbVertices; v++)
      {
        if ((v != u) && (g->mat[u][v] != INT_MAX) && (D[v] > D[u] + g->mat[u][v]))
        {

          D[v] = D[u] + g->mat[u][v];
          Pred[v] = u;
          majDijkstra(F, D, v, k, indiceDansF);
        }
      }
    }
  }
}

void majDijkstra(int *F, int *D, int v, int k, int *indiceDansF)
{
  int i = indiceDansF[v] - k;
  int c;
  while (((i / 2) >= 1) && (D[F[(i / 2) + k]] > D[F[i + k]]))
  {

    c = F[i + k];
    F[i + k] = F[(i / 2) + k];
    F[(i / 2) + k] = c;

    indiceDansF[F[i + k]] = i + k;
    indiceDansF[F[(i / 2) + k]] = (i / 2) + k;

    i = i / 2;
  }
  if (D[F[(i / 2) + k]] > D[F[i + k]])
  {

    c = F[i + k];
    F[i + k] = F[(i / 2) + k];
    F[(i / 2) + k] = c;

    indiceDansF[F[i + k]] = i + k;
    indiceDansF[F[(i / 2) + k]] = (i / 2) + k;
  }
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

//Fonctions qui effacent l'écran si des constantes sont définies

#ifdef SHOW_RESULT_PROGRAM1
void showProgramme1(int i){ }
#else
void showProgramme1(int i){

  CLRSCR(); //effacent l'écran
  printf("\nexécution du programme 1 avec %d nodes\n\n", i);

}
#endif

#ifdef SHOW_RESULT_PROGRAM2
void showProgramme2(int i){ }
#else
void showProgramme2(int i){

  CLRSCR(); //effacent l'écran
  printf("\nexécution du programme 1 avec %d nodes\n\n", i);
  printf("--------------------------------------------------\n");
  printf("\nexécution du programme 2 avec %d nodes\n\n", i);

}
#endif