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

  //Affichage sortie Standard
  char command[200];
  snprintf(command, sizeof(command), "cat %s", file_name);
  system(command);
}

void showGraphPNG(const char *filename, T_graphLA *g)
{
  char file_name[50];
  snprintf(file_name, sizeof(file_name), "%s.dot", filename);
  writeDotGraph(file_name, g);
  char command[200];

  mkdir("output/png", 0777);

  snprintf(command, sizeof(command), "dot output/dot/%s.dot -T png -o output/png/%s.png", filename, filename);
  system(command);
  printf("Création du fichier : output/png/%s.png\n", filename);
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
int checkExtension(char *extensionToHave, const char *filename)
{
  char cp2[50];
  strcpy(cp2, filename);
  char *extension = strtok(cp2, ".");
  extension = strtok(NULL, ".");

  if (extension == NULL || strcmp(extension, extensionToHave) != 0)
  {
    printf("Verifiez l'extension du fichier ... L'argument saisi ne semble mené vers aucun/mauvais fichier ...\n");
    exit(-1);
  }
  return 0;
}
T_graphLA *adjToGraph(const char *filename)
{
  FILE *fp;
  checkExtension("adj", filename);
  CHECK_IF(fp = fopen(filename, "r"), NULL, "L'argument saisi ne semble mené vers aucun fichier ...");

  int ligne = 0, column = 0, size = 0;
  const char *separators = "\t\n";
  char line[100];

  // Parcours le file pour connaitre nb_lignes = graph size
  FILE *fp_copy;
  CHECK_IF(fp_copy = fopen(filename, "r"), NULL, "L'argument saisi ne semble mené vers aucun fichier ...");

  while (fgets(line, 100, fp_copy) != NULL)
  {
    size++;
  }
  fclose(fp_copy);

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

T_graphLA *laToGraph(const char *filename)
{
  FILE *fp;
  checkExtension("la", filename);
  CHECK_IF(fp = fopen(filename, "r"), NULL, "L'argument saisi ne semble mené vers aucun fichier ...");

  int ligne = 0, size = 0;
  const char *separators = "\t\n";
  char line[100];

  // Parcours le file pour connaitre nb_lignes = graph size
  FILE *fp_copy;
  CHECK_IF(fp_copy = fopen(filename, "r"), NULL, "L'argument saisi ne semble mené vers aucun fichier ...");

  while (fgets(line, 100, fp_copy) != NULL)
  {
    size++;
  }
  fclose(fp_copy);

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

void adjToLa(const char *fichierdepart)
{
  mkdir("output",0777);
  mkdir("output/la/", 0777);
  FILE *fp, *dep;
  checkExtension("adj", fichierdepart);
  CHECK_IF(dep = fopen(fichierdepart, "r"), NULL, "L'argument saisi ne semble mené vers aucun fichier ...");

  char cp[50], filename[50];
  strcpy(cp, fichierdepart);
  snprintf(filename, sizeof(filename), "output/la/%s.la", strtok(cp + 10, "."));
  CHECK_IF(fp = fopen(filename, "w"), NULL, "L'argument saisi ne semble pas bien être formulé ...");
  printf("création du fichier : %s \n", filename);

  int ligne = 0, column = 0;
  const char *separators = "\t\n_";
  char line[100];

  while (fgets(line, 100, dep) != NULL)
  {
    fprintf(fp, "%d\t", ligne);
    char *token = strtok(line, separators);
    while (token != NULL)
    {
      if (isNumber(token) == 1 && atoi(token) != 0 && token != 0)
      {
        fprintf(fp, "%d_%s\t", column, token);
      }
      token = strtok(NULL, separators);
      column++;
    }
    fprintf(fp, "\n");
    ligne++;
    column = 0;
  }
  fclose(fp);
  fclose(dep);
}

void laToAdj(const char *fichierdepart)
{
  mkdir("output",0777);
  mkdir("output/adj/", 0777);
  FILE *fp, *dep;
  checkExtension("la", fichierdepart);
  CHECK_IF(dep = fopen(fichierdepart, "r"), NULL, "L'argument saisi ne semble mené vers aucun fichier ...");

  char cp[50], filename[50];
  strcpy(cp, fichierdepart);
  snprintf(filename, sizeof(filename), "output/adj/%s.adj", strtok(cp + 9, "."));
  CHECK_IF(fp = fopen(filename, "w"), NULL, "L'argument saisi ne semble pas bien être formulé ...");
  printf("création du fichier : %s \n", filename);

  int ligne = 0, column = 0;
  int size = 0;
  const char *separators = "\t\n_";
  char line[100];

  FILE *fp_copy;
  CHECK_IF(fp_copy = fopen(fichierdepart, "r"), NULL, "fopen");
  while (fgets(line, 100, fp_copy) != NULL)
  {
    size++;
  }
  fclose(fp_copy);

  while (fgets(line, 100, dep) != NULL)
  {
    char *token = strtok(line, separators);
    token = strtok(NULL, separators);

    for (column = 0; column < size; column++)
    {
      if (token != NULL && atoi(token) == column)
      {
        token = strtok(NULL, separators); //prendre le poids associé au sommet
        fprintf(fp, "%d\t", atoi(token));
        token = strtok(NULL, separators);
      }

      else if (column == ligne)
      {
        fprintf(fp, "0\t");
      }
      else
      {
        fprintf(fp, "i\t");
      }
    }
    fprintf(fp, "\n");
    ligne++;
  }
  fclose(fp);
  fclose(dep);
}

int startsWith(const char *pre, const char *str)
{
  size_t lenpre = strlen(pre),
         lenstr = strlen(str);
  return lenstr < lenpre ? 1 : memcmp(pre, str, lenpre) == 0;
}

int main(int argc, char **argv)
{
  T_graphLA *g;
  if (argc == 4)
  {
    //on considère les chemins vers les fichiers commencant par input et non par ./input par exemple
    // car peut causer des soucis lors de la vérification de l'extension etc...
    if (startsWith("./", argv[3]))
    {
      argv[3] += 2; //suppression du ./
    }

    if (strcmp(argv[1], "-dot") == 0)
    {

      //EXEMPLE 1
      if (strcmp(argv[2], "-a") == 0)
      {
        g = adjToGraph(argv[3]);

        char cp[50];
        strcpy(cp, argv[3]);

        char *filename = strtok(cp + 10, ".");
        showGraphPNG(filename, g);

        // EXEMPLE 2
      }
      else if (strcmp(argv[2], "-l") == 0)
      {
        g = laToGraph(argv[3]);

        char cp[50];
        strcpy(cp, argv[3]);

        char *filename = strtok(cp + 9, ".");
        showGraphPNG(filename, g);
      }
      else
      {
        printf("\n\tL'argument : %s n'est pas connu ...\n\tVeuillez choisir un 2e argument valable parmi : -a -l\n\n", argv[2]);
      }

      // EXEMPLE 3
    }
    else if (strcmp(argv[1], "-la") == 0)
    {
      if (strcmp(argv[2], "-a") == 0)
      {
        adjToLa(argv[3]);
      }
      else
      {
        printf("\n\tL'argument : %s n'est pas connu ...\n\tVeuillez choisir un 2e argument valable parmi : -a\n\n", argv[2]);
      }
      // EXEMPLE 4
    }
    else if (strcmp(argv[1], "-adj") == 0)
    {
      if (strcmp(argv[2], "-l") == 0)
      {
        laToAdj(argv[3]);
      }
      else
      {
        printf("\n\tL'argument : %s n'est pas connu ...\n\tVeuillez choisir un 2e argument valable parmi : -l\n\n", argv[2]);
      }
    }
    else
    {
      printf("\n\tL'argument : %s n'est pas connu ...\n\tVeuillez choisir un 1er argument valable parmi : -dot -la -adj\n\n", argv[1]);
    }
  }
  else
  {
    printf("\n\t3 paramètres doivent être saisis\n\n");
  }
  return 0;

  /*T_graphLA *g5 = newGraphLA(4);
  addEdge(g5, 0, 2, 3);
  addEdge(g5, 2, 3, 4);
  addEdge(g5, 3, 1, 1);
  addEdge(g5, 1, 2, 2);
  addEdge(g5, 0, 1, 9);

  //printGraph(g5);
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

  T_graphLA *g1 = adjToGraph("input/adj/graph1.adj");
  showGraphPNG("graph1", g1);

  T_graphLA *g2 = adjToGraph("input/adj/graph2.adj");
  showGraphPNG("graph2", g2);

  T_graphLA *g3 = adjToGraph("input/adj/graph3.adj");
  showGraphPNG("graph3", g3);
  
  matTolist("output/graph2.la","input/adj/graph1.adj");*/

  /*laToAdj("input/la/graph1.la");
  laToAdj("input/la/graph2.la");
  laToAdj("input/la/graph3.la");
  
  adjToLa("input/adj/graph1.adj");
  adjToLa("input/adj/graph2.adj");
  adjToLa("input/adj/graph3.adj");*/

  return 0;
}
