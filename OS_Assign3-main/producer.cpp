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
#include "main.hpp"

using namespace std;

void prod()
{

    int x = 2;
    while (x--)
    {
        //cout << "HI" << endl;
        // Sleep for 50 seconds
        sleep(5);
        // Generate random number of nodes to add
        int m = rand() % 21 + 10; // update to rand()%21 +10
        // Add m new nodes
        for (int i = 0; i < m; i++)
        {
            // Select number of existing nodes to connect to
            long unsigned int k = rand() % 20 + 1; //update to 20
            // Select existing nodes to connect to based on popularity
            vector<int> popularNodes;
            for (int j = 0; j < *noofNodes; j++)
            {

                for (int l = 0; l < ver[j].deg; l++)
                {
                    popularNodes.push_back(j);
                }
            }
            // Connect new node to k existing nodes

            set<int> s;
            while (s.size() < k)
            {
                s.insert(popularNodes[rand() % popularNodes.size()]);
            }
            for (auto &it : s)
            {
                int nodeToConnect = it;
                int from_node = nodeToConnect;
                int to_node = *noofNodes;

                Node *temp1 = new (edge + (*noofEdges)) Node;
                temp1->vertex = to_node;
                temp1->next = NULL;

                (*noofEdges)++;
                addEdge((Node *)(ver + from_node), temp1);

                Node *temp2 = new (edge + (*noofEdges)) Node;
                temp2->vertex = from_node;
                temp2->next = NULL;

                (*noofEdges)++;
                addEdge((Node *)(ver + to_node), temp2);
            }
           
            (*noofNodes)++;
        }
        for (int i = 0; i < *noofNodes; i++) {
            Node* currentNode = (ver+i)->next;
            cout << ver[i].vertex << ": ";
            while (currentNode != NULL) {
                cout << currentNode->vertex << " ";
                currentNode = currentNode->next;
            }
            cout << endl;
    }
    }
    return;
}
