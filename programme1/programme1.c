#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../include/treatmentFiles.h"


int main(int argc, char **argv)
{
  if (argc != 4)
  {
    printf("\n\t3 paramètres doivent être saisis\n\n");
    return 0;
  }

  int sr = atoi(argv[2]);
  int dst = atoi(argv[3]);

  T_graphMD *g = NULL;
  g = readFile(argv[1]);

  int *D = (int *)malloc(g->nbVertices * sizeof(int));
  int *T = (int *)malloc(g->nbVertices * sizeof(int));
  int *Pred = (int *)malloc(g->nbVertices * sizeof(int));

  int i;
  for (i = 0; i < g->nbVertices; i++)
  {
    T[i] = 0;
  }

  char *filename = strtok(argv[1], ".");

  for (i = 6; filename[i] != '\0'; i++)
    filename[i - 6] = filename[i];

  filename[i - 6] = '\0';

  dijkstra(g, D, T, Pred, sr, filename);

  if (dst < g->nbVertices)
  {
    printf("\n%d\n", D[dst]);
    showResult(D, Pred, sr, dst);
    printf("\n\n");
  }
  else
  {
    printf("This is impossible\n\n");
  }

  free(D);
  free(T);
  free(Pred);
  return 0;
}

void showResult(int *D, int *Pred, int src, int dst) {

  if (dst != src)
  {
    if (Pred[dst] == -1) {
      printf("\nLe chemin n'existe pas\n");
    }
    else {
      showResult(D, Pred, src, Pred[dst]);
      printf(" -> %d", dst);
    }
  }
  else
    printf("%d", src);
}

void compareItem(T_graphMD *g, int *D, int *Pred, int x, int y)
{
  if (D[x] + g->mat[x][y] < D[y])
  {
    D[y] = D[x] + g->mat[x][y];
    Pred[y] = x;
  }
}

void dijkstra(T_graphMD *g, int *D, int *T, int *Pred, int sr, char *filename)
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
      createPNG(filename, g, u, T, k);
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

  createPNG(filename, g, -1, T, k);

  // create a video of successive generated PNG files
  char fileNameComplet[150] = "";
  sprintf(fileNameComplet, "output/pngFiles/%s/%s", filename, filename);

  char command[400] = "";
  sprintf(command, "ffmpeg -r 1 -s 1920x1080 -i %s_%%1d.png -vcodec libx264 -crf 25 -y -loglevel quiet  %s.mp4", fileNameComplet, fileNameComplet);

  system(command);
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