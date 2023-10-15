#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <time.h>

#include "main.hpp"


using namespace std;
#define MAX_NODES 10000
#define MAX_EDGES 200000
 
int INF = 1e9;
Node* ver;
Node* edge;
// int* arr;
int *noofNodes;
int *noofEdges;

void addEdge(Node *src,Node* dest){
    if(src->next==NULL){
        src->next=dest;
    }
    else{
        Node* temp=src->next;
        src->next=dest;
        dest->next=temp;
    }
        src->deg++;
}

int main(int argc, char* argv[])
{
    srand(time(0));
    int optimized = 0;
     if(argc > 1 && strcmp(argv[1],"-optimize") == 0)
    {
        optimized = 1;
    }
    int n_ver=0,n_edge=0;
    key_t key1, key2,key3;
    key1 = 4201;
    key2 = 5201;
    key3 = 6201;
    //cout<<sizeof(Node);  //24 ans
    int shm_1 = shmget(key1, MAX_NODES*(sizeof(Node)), IPC_CREAT | 0666);
    int shm_2 = shmget(key2,MAX_EDGES*(sizeof(Node)), IPC_CREAT | 0666);
    int shm_3 = shmget(key3,2*sizeof(int), IPC_CREAT | 0666);
   
    ver  = (Node*)shmat(shm_1,NULL,0);
    edge = (Node*)shmat(shm_2,NULL,0);
    noofNodes  = (int*)shmat(shm_3,NULL,0);
    noofEdges = noofNodes+1;
    int from_node, to_node;
    set<int> s;
    for(int i=0;i<MAX_NODES;i++){
        Node* t=new(ver+i) Node;
        ver[i].vertex=i;
        ver[i].next=NULL;  
    }
    ifstream file("facebook_combined.txt"); //facebook_combined.txt
    int x=0;
    while (file >> from_node >> to_node)
    {    
        Node* temp1=new(edge+n_edge) Node;
        temp1->vertex=to_node;
        temp1->next=NULL;
        n_edge++;
        (*noofEdges)++;
        addEdge((Node*)(ver+from_node),temp1);
      
        Node* temp2=new(edge+n_edge) Node;
        temp2->vertex=from_node;
        temp2->next=NULL;
        n_edge++;
        (*noofEdges)++;
        addEdge((Node*)(ver+to_node),temp2);

        s.insert(from_node);
        s.insert(to_node);
    }

    int num_nodes=s.size();
    *noofNodes = num_nodes;
    pid_t pid = fork();
    if (pid == 0)
    {
        prod();
        //shmdt();
        shmdt(ver);
        shmdt(edge);
        shmdt(noofNodes);
        exit(0);
    }
    else if (pid > 0)
    {
        cout <<"1"<<endl;
        //vector<pid_t> cpid(10);
        pid_t cpid;
        for(int i=0;i<10;i++)
        {
            vector<int> dist(MAX_NODES,INF);
            vector<int> parent(MAX_NODES,-1);
            int prev=0;
            cpid=fork();
            if(cpid==0)
            {
                int k=0;
                while(k<=3)
                {
                    sleep(3); // update to 30  
                    k++;  
                    cout << k << endl;
                    if(optimized == 0) shortest_paths(i);
                   else if(optimized == 1)  
                    {
                        optimize(i,dist,parent,prev,k);
                        prev=*noofNodes;
                    }     
                }
                exit(0);
            }
        }
        for(int i=0;i<10;i++) wait(NULL);
        waitpid(pid, NULL, 0);
        /* for (int i = 0; i < *noofNodes; i++) {
            Node* currentNode = (ver+i)->next;
            cout << ver[i].vertex << ": ";
            while (currentNode != NULL) {
                cout << currentNode->vertex << " ";
                currentNode = currentNode->next;
            }
            cout << endl;
        }*/
    }
    else
    {
        cerr << "Failed to create producer process!" << endl;
        return 1;
    }

   
    
    shmdt(ver);
    shmdt(edge);
    shmdt(noofNodes);
    shmctl(shm_1,IPC_RMID,NULL);
    shmctl(shm_2,IPC_RMID,NULL);
    shmctl(shm_3,IPC_RMID,NULL);
    return 0;

}
