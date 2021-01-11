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
  mkdir("output", 0777);  //creation des répertoires
  mkdir("output/la/", 0777); 

  //ouverture des fichiers 
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

  while (fgets(line, 100, dep) != NULL)//parcourt les lignes de la matrice 
  {
    fprintf(fp, "%d\t", ligne); //écrit le numéro de la ligne 
    char *token = strtok(line, separators); //recupère 1er elements de la logne
    while (token != NULL)
    {
      if (isNumber(token) == 1 && atoi(token) != 0 && token != 0)//si c'est le poids d'un arc
      {
        fprintf(fp, "%d_%s\t", column, token);//écrit avec le format choisi
      }
      token = strtok(NULL, separators);//prend le 2eme éléments
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
  mkdir("output", 0777);   //creation des répertoires
  mkdir("output/adj/", 0777);

   //ouverture des fichiers 
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
  const char *separators = "\t\n_";//définition des séparateurs
  char line[100];

  FILE *fp_copy;
  CHECK_IF(fp_copy = fopen(fichierdepart, "r"), NULL, "fopen");
  while (fgets(line, 100, fp_copy) != NULL)//taille de la matrice
  {
    size++;
  }
  fclose(fp_copy);

  while (fgets(line, 100, dep) != NULL)//parcourt des lignes
  {
    char *token = strtok(line, separators);//récupère le 1ER élément qui est n° de la ligne
    token = strtok(NULL, separators);// récupère le 2eme élément qui indique la présence d'un arc

    for (column = 0; column < size; column++) //création des colonnes 
    {
      if (token != NULL && atoi(token) == column)//si un arc existe 
      {
        token = strtok(NULL, separators); //prendre le poids associé au sommet
        fprintf(fp, "%d\t", atoi(token));
        token = strtok(NULL, separators);
      }

      else if (column == ligne)//si on est en diagonale
      {
        fprintf(fp, "0\t");
      }
      else//si aucun arc existe
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

}
