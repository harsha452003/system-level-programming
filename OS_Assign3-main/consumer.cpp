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
#include <queue>
#include <vector>
#include <stack>
#include "main.hpp"
#include <iostream>

using namespace std;

void shortest_paths(int cid)
{
     int n=(*noofNodes);
     //cout << n << endl;
     int batch=n/10;
     vector<int> vis(n,0);
     vector<int> parent(n,-1);
     queue<int> q;
     int end = ((cid!=9)?(batch*(cid+1)):n);
     for(int i=batch*cid;i<end;i++)
     {
            q.push(i);
            vis[i]=1;
            parent[i]=i;
     }
     while(!q.empty())
     {
        int u=q.front();
        q.pop();
        Node* ulist = (ver+u)->next;
        while(ulist!=NULL)
        {
            if(vis[ulist->vertex]==0) 
            {
                vis[ulist->vertex]=1;
                parent[ulist->vertex]=u;
                q.push(ulist->vertex);
            }
            ulist=ulist->next;
        }  
     }
     string file_name = "consumer_"+to_string(cid)+".txt";
     ofstream file(file_name);
     if(!file.is_open())
     {
       cout << "Unable to open file" << endl;
     }
     else
     {
          for(int i=0;i<n;i++)
          {
                if(parent[i]!=i && vis[i]==1)
                {
                    int node=i;
                    stack<int> path;
                    while (parent[node] != node) 
                    {
                        path.push(node);
                        node = parent[node];
                    }
                    path.push(node);

                    while (!path.empty())
                    {

                        if(path.size()!=1)
                        {
                        file << path.top() << "->";
                        //cout << path.top() << "->";
                        }
                        else
                        {
                        file << path.top() << endl;
                        //cout << path.top() << endl << endl; 
                        }
                        path.pop();
                    }
            
                }
          }
     }
}

void optimize(int cid,vector<int> &dist,vector<int> &parent,int prev,int k)
{
   int n=*noofNodes;
   queue<int> q;
   vector<int> update(n,0);
   if(prev==0)
   {
    //    for(int i=cid;i<n;i=i+10)
    //    {
    //       update[i]=1;
    //       q.push(i);
    //       dist[i]=0;
    //       parent[i]=i;
    //    }
        int batch=n/10;
        int end = ((cid!=9)?(batch*(cid+1)):n);
        for(int i=batch*cid;i<end;i++)
        {
                q.push(i);
                dist[i]=0;
                update[i]=1;
                parent[i]=i;
        }
   }
   else
   { 
         for(int i=prev;i<n;i++)
         {
            if(i%10==cid) 
            {   
                update[i]=1;
                dist[i]=0;
                parent[i]=i;
            }
            else
            {
               Node* u=(ver+i)->next;       
               int min=dist[u->vertex];
               while(u!=NULL)
               {
                  if(min>=dist[u->vertex]) 
                  {
                    min=dist[u->vertex];
                    parent[i]=u->vertex;
                  }
                  u=u->next;
               } 
               update[i]=1;
               dist[i]=min+1;
            }
            q.push(i);
         }
   }
  
   while(!q.empty())
   {
     int u=q.front();
     q.pop();
    
     Node* ulist = (ver+u)->next;
     while(ulist!=NULL)
     {
        if(dist[ulist->vertex]>dist[u]+1)
        {
            dist[ulist->vertex]=dist[u]+1;
            update[ulist->vertex]=1;
            parent[ulist->vertex]=u;
            q.push(ulist->vertex);
        }
        ulist=ulist->next;
     }
   }
   
     string file_name = "consumer_"+to_string(cid)+".txt";
     ofstream file(file_name,ofstream::app);
     if(!file.is_open())
     {
       cout << "Unable to open file" << endl;
     }
     else
     {
          file << "\nIteration " << k << ": \n\n";
          for(int i=0;i<n;i++)
          {
                if(parent[i]!=i && update[i]==1)
                {
                    int node=i;
                    stack<int> path;
                    while (parent[node] != node) 
                    {
                        path.push(node);
                        node = parent[node];
                    }
                    path.push(node);

                    while (!path.empty())
                    {

                        if(path.size()!=1)
                        {
                        file << path.top() << "->";
                        //cout << path.top() << "->";
                        }
                        else
                        {
                        file << path.top() << endl;
                        //cout << path.top() << endl << endl; 
                        }
                        path.pop();
                    }
            
                }
          }
     }

}
