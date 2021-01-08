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
void showGraph(const char *, T_graphLA *);
void listTomat(const char *,const char *);
void matTolist(const char *,const char *);