#include "include/adjacence_list.h"
#include "include/check.h"
#include <limits.h>

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
  newNode->pNext = g->tAdj[s];
  g->tAdj[s] = newNode;

  /*  newNode = createNode(s,weight);
    newNode->pNext = g->tAdj[d];
    g->tAdj[d] = newNode;*/
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

void showGraph(const char *filename, T_graphLA *g)
{
  // Ecrit le format dot complet (entête+structure) dans un fichier
  // Utilise dumpGraph

  FILE *fp;
  CHECK_IF(fp = fopen(filename, "w"), NULL, "fopen");

  fprintf(fp, "digraph graphe {\n");
  fprintf(fp, "rankdir = LR;\n");
  fprintf(fp, "node [fontname=\"Arial\", shape = circle, color=lightblue, style=filled];\n");
  fprintf(fp, "edge [color=red]\n");
  dumpGraph(fp, g);
  fprintf(fp, "}\n");
}

int main(void)
{
  T_graphLA *g = newGraphLA(4);
  addEdge(g, 0, 2, 3);
  addEdge(g, 0, 1, 9);

  addEdge(g, 2, 3, 4);
  addEdge(g, 3, 1, 1);
  addEdge(g, 1, 2, 2);

  printGraph(g);
  showGraph("graph.dot", g);

  T_graphLA *g2 = newGraphLA(8);
  addEdge(g2, 1, 2, 12);
  addEdge(g2, 1, 6, 15);
  addEdge(g2, 1, 7, 20);
  addEdge(g2, 2, 3, 21);
  addEdge(g2, 3, 5, 3);
  addEdge(g2, 3, 8, 19);
  addEdge(g2, 4, 3, 7);
  addEdge(g2, 4, 8, 7);
  addEdge(g2, 5, 4, 13);
  addEdge(g2, 5, 8, 14);
  addEdge(g2, 6, 3, 17);
  addEdge(g2, 6, 5, 28);
  addEdge(g2, 6, 7, 4);
  addEdge(g2, 7, 5, 18);
  addEdge(g2, 7, 8, 45);

  showGraph("graph2.dot", g2);

  return 0;
}
