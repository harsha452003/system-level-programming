#ifndef MAIN_H
#define MAIN_H

struct Node {
    int c_id;
    int vertex;
    int deg;
    Node* next;
};

extern struct Node* ver;
extern struct Node* edge;
extern int *noofEdges;
extern int *noofNodes;

void shortest_paths(int);
void optimize(int,std::vector<int>&,std::vector<int>&,int,int);
void addEdge(Node *src,Node* dest);
void prod();
#endif