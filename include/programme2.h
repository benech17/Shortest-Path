#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    unsigned int vertexNumber;
    int weight;
    struct node* pNext;
}T_node, *T_list;


typedef struct{
    unsigned int nbVertices;
    T_list *tAdj;
}T_graphLA;



T_graphLA * newGraphLA (int);
T_node* createNode (int,int);
void addEdge(T_graphLA*,int,int,int);
void printGraph(T_graphLA*);
void dumpGraph(FILE *, T_graphLA *);
void showGraphPNG(const char *, T_graphLA *);

T_graphLA *adjToGraph(const char *);
T_graphLA *laToGraph(const char *filename);

void laToAdj(const char *);
void adjToLa(const char *);

int isNumber(char *);
int checkExtension(char*, const char*);
int startsWith(const char *, const char *);

