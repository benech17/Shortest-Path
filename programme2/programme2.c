#include "../include/programme2.h"
#include "../include/check.h"
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

// Complexity in O( n+m) where n is the number of nodes and m the nulber of vertices

T_graphLA *newGraphLA(int n)
{
  T_graphLA *g;
  g = (T_graphLA *)malloc(sizeof(T_graphLA));
  g->nbVertices = n;
  g->tAdj = (T_list *)malloc(n * sizeof(T_list));
  for (int i = 0; i < n; i++)
  {
    g->tAdj[i] = NULL;
  }
  return g;
}
T_node *createNode(int value, int weight)
{
  T_node *newNode = malloc(sizeof(T_node));
  newNode->vertexNumber = value;
  newNode->weight = weight;
  newNode->pNext = NULL;
  return newNode;
}

void addEdge(T_graphLA *g, int s, int d, int weight)
{
  T_node *newNode = createNode(d, weight);
  // insert at the right place to have an ordered linked list
  if (g->tAdj[s] == NULL || g->tAdj[s]->vertexNumber >= d)
  {
    newNode->pNext = g->tAdj[s];
    g->tAdj[s] = newNode;
  }
  else
  {
    T_node *tmp; // noeud temporaire pour parcourir la liste
    tmp = g->tAdj[s];
    while (tmp->pNext != NULL && tmp->pNext->vertexNumber < d)
    {
      tmp = tmp->pNext;
    }
    newNode->pNext = tmp->pNext;
    tmp->pNext = newNode;
  }
}

void printGraph(T_graphLA *g)
{
  for (int v = 0; v < g->nbVertices; v++)
  {
    T_node *temp = g->tAdj[v];
    printf("\n Vertex %d\n: ", v);
    while (temp)
    {
      printf("%d |%d-> ", temp->vertexNumber, temp->weight);
      temp = temp->pNext;
    }
    printf("\n");
  }
}

void dumpGraph(FILE *fp, T_graphLA *g)
{
  // Affiche la structure du graphe dans un FILE *
  for (int v = 0; v < g->nbVertices; v++)
  {
    T_node *tmp = g->tAdj[v];
    while (tmp != NULL)
    {
      fprintf(fp, "\t%d -> %d [label = \"%d\"];\n", v, tmp->vertexNumber, tmp->weight);
      tmp = tmp->pNext;
    }
  }
}

void writeDotGraph(const char *filename, T_graphLA *g)
{
  // Ecrit le format dot complet (entête+structure) dans un fichier
  mkdir("output", 0777);
  mkdir("output/dot", 0777);

  char file_name[50];
  snprintf(file_name, sizeof(file_name), "output/dot/%s", filename);
  FILE *fp;
  CHECK_IF(fp = fopen(file_name, "w"), NULL, "fopen");

  fprintf(fp, "digraph graphe {\n");
  fprintf(fp, "rankdir = LR;\n");
  fprintf(fp, "node [fontname=\"Arial\", shape = circle, color=lightblue, style=filled];\n");
  fprintf(fp, "edge [color=red]\n");
  dumpGraph(fp, g);
  fprintf(fp, "}\n");
  fclose(fp);
}

void showGraphPNG(const char *filename, T_graphLA *g)
{
  char file_name[50];
  snprintf(file_name, sizeof(file_name), "%s.dot", filename);
  writeDotGraph(file_name, g);
  char command[200];

  mkdir("output/png", 0777);

  snprintf(command, sizeof(command), "dot output/dot/%s.dot -T png -o output/png/%s.png", filename, filename);
  printf("%s\n", command);
  system(command);
}

int isNumber(char *token)
{
  for (int i = 0; i < strlen(token); i++)
  {
    if (!isdigit(token[i]))
    {
      return 0;
    }
  }
  return 1;
}
T_graphLA *matToAdj(const char *filename)
{
  FILE *fp;
  CHECK_IF(fp = fopen(filename, "r"), NULL, "fopen");

  int ligne = 0, column = 0, size = 0;
  const char *separators = "\t\n";
  char line[100];

  // Parcours le file pour connaitre nb_lignes = graph size
  FILE *fp_copy;
  CHECK_IF(fp_copy = fopen(filename, "r"), NULL, "fopen");

  while (fgets(line, 100, fp_copy) != NULL)
  {
    size++;
  }
  T_graphLA *g = newGraphLA(size);

  while (fgets(line, 100, fp) != NULL)
  {
    char *token = strtok(line, separators);
    while (token != NULL)
    {
      if (isNumber(token) == 1 && atoi(token) != 0)
      {
        addEdge(g, ligne, column, atoi(token));
      }
      token = strtok(NULL, separators);
      column++;
    }
    ligne++;
    column = 0;
  }
  fclose(fp);
  return g;
}

T_graphLA *laToAdj(const char *filename)
{
  FILE *fp;
  CHECK_IF(fp = fopen(filename, "r"), NULL, "fopen");

  int ligne = 0, size = 0;
  const char *separators = "\t\n";
  char line[100];

  // Parcours le file pour connaitre nb_lignes = graph size
  FILE *fp_copy;
  CHECK_IF(fp_copy = fopen(filename, "r"), NULL, "fopen");

  while (fgets(line, 100, fp_copy) != NULL)
  {
    size++;
  }
  T_graphLA *g = newGraphLA(size);

  while (fgets(line, 100, fp) != NULL) //parcours les lignes du fichier fp
  {
    char *token = strtok(line, separators); // numéro de ligne
    while (token != NULL)
    {
      if (!isNumber(token))
      {
        char *dest = strtok_r(token, "_", &token); //strtok_r est ré-entrant
        if (isNumber(dest) && isNumber(token))
        {
          addEdge(g, ligne, atoi(dest), atoi(token));
        }
      }
      token = strtok(NULL, separators);
    }
    ligne++;
  }
  fclose(fp);
  return g;
}

int main(void)
{
  T_graphLA *g5 = newGraphLA(4);
  addEdge(g5, 0, 2, 3);
  addEdge(g5, 2, 3, 4);
  addEdge(g5, 3, 1, 1);
  addEdge(g5, 1, 2, 2);
  addEdge(g5, 0, 1, 9);

  printGraph(g5);/*
  //writeDotGraph("graph.dot", g);
  showGraphPNG("graph5", g5);

  T_graphLA *g4 = newGraphLA(8);
  addEdge(g4, 1, 2, 12);
  addEdge(g4, 1, 6, 15);
  addEdge(g4, 2, 3, 21);
  addEdge(g4, 3, 5, 3);
  addEdge(g4, 6, 5, 28);
  addEdge(g4, 3, 8, 19);
  addEdge(g4, 4, 3, 7);
  addEdge(g4, 4, 8, 7);
  addEdge(g4, 5, 8, 14);
  addEdge(g4, 5, 4, 13);
  addEdge(g4, 1, 7, 20);
  addEdge(g4, 6, 3, 17);
  addEdge(g4, 6, 7, 4);
  addEdge(g4, 7, 5, 18);
  addEdge(g4, 7, 8, 45);
  showGraphPNG("graph4", g4);

  T_graphLA *g1 = laToAdj("../input/la/graph1.la");
  showGraphPNG("graph1", g1);

  T_graphLA *g2 = laToAdj("../input/la/graph2.la");
  showGraphPNG("graph2", g2);

  T_graphLA *g3 = laToAdj("../input/la/graph3.la");
  showGraphPNG("graph3", g3);*/

  //laToAdjFile("../input/la/graph1_copy.la");

  return 0;
}
