#include <stdlib.h>
#include <stdio.h>

// Complexity in O( n+m) where n is the number of nodes and m the nulber of vertices

typedef struct node {
    unsigned int vertexNumber;
    int weight;
    struct node* pNext;
}T_node, *T_list;

typedef struct{
    unsigned int nbVertices;
    T_list *tAdj;
}T_graphLA;

T_graphLA * newGraphLA (int n){
    T_graphLA* g ;
    g=(T_graphLA *) malloc(sizeof(T_graphLA));
    g->nbVertices = n;
    g->tAdj = (T_list *) malloc (n*sizeof(T_list));
    for (int i=0;i<n;i++){
        g->tAdj[i] = NULL;
    }
    return g;
}
T_node* createNode (int value,int weight){
    T_node* newNode = malloc(sizeof(T_node));
    newNode->vertexNumber = value;
    newNode->weight = weight;
    newNode->pNext =NULL;

}
void addEdge(T_graphLA* g,int s,int d,int weight){
    T_node* newNode = createNode(d,weight);
    newNode->pNext = g->tAdj[s];
    g->tAdj[s] = newNode;

  /*  newNode = createNode(s,weight);
    newNode->pNext = g->tAdj[d];
    g->tAdj[d] = newNode;*/
}
void printGraph(T_graphLA* g) {
  for (int v = 0; v < g->nbVertices; v++) {
    T_node* temp = g->tAdj[v];
    printf("\n Vertex %d\n: ", v);
    while (temp) {
      printf("%d |%d-> ", temp->vertexNumber , temp->weight);
      temp = temp->pNext;
    }
    printf("\n");
  }
}
int main(void){
    T_graphLA* g = newGraphLA(4);
    addEdge(g,0,1,9);
    addEdge(g,0,2,3);
    addEdge(g,2,3,4);
    addEdge(g,3,1,1);
    addEdge(g,1,2,2);
   

    printGraph(g);
    return 0;
}


