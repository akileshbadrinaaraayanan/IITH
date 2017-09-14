//Minimum Vertex cover using Primal Dual. LO Assignment 3. Implemented by Akilesh B, CS13B1042. 
//Required header files
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

//Main function
int main()
{	
	//Taking input from a file	
	FILE *fp;
	fp = fopen("input1.txt", "r");

	if (fp == NULL)
	{   
		printf("error in opening the file\n"); 
        exit(-1);
    }

    int n, var1, var2;
    vector<int> edges[2];
	vector<int> adj_edges[2];

	//First take n, number of vertices
    fscanf(fp, "%d", &n);
    int w[n];

    //w is the weight array.
    for(int i=0; i<n; i++)
    {
    	fscanf(fp, "%d", &var1);
    	w[i] = var1;
    }

    //Adjacency list as input
    int adj[n][n];

    for(int i=0; i<n; i++)
    {
    	for(int j=0; j<n; j++)
    	{
    		fscanf(fp, "%d", &var2);
    		adj[i][j] = var2;
    	}
    }

    //Update the edges and edge count from adjacency list.
    int edge_count = 0;
    for(int i=0; i<n; i++)
    {
    	for(int j=0; j<n; j++)
    	{
    		if((i < j) && (adj[i][j] == 1))
    		{
    			edges[0].push_back(i);
				edges[1].push_back(j);
				edge_count++;
    		}
    	}
    }
    // Print the weights --- Just my checking
    for(int i=0; i<n; i++)
    {
    	cout << w[i] << "\t";
    }
    cout << endl;

    for(int i=0; i<n; i++)
    {
    	for(int j=0; j<n; j++)
    	{
    		cout << adj[i][j] << "\t";
    	}
    	cout << "\n";
    }
    
	int m = edge_count;
	
	//y[e] = Weight of edge e
	int y[m];
	
	for(int i=0; i<m; i++)	
	{
		y[i]=0;
	}

	//S is the vertex cover
  	int S[n];
  	for(int i=0; i<n; i++)
  	{
  		S[i] = 0;
  	}	

  	// s is sum of all the edges emerging from i
	int s[n];
	for(int i=0; i<n; i++)	
	{
		s[i]=0;
	}

	//The variable inserted is needed because, when s value of both the vertices of an edge reaches its maximum, then both will be included in the vertex cover
	//But edge should not be included twice in the adj_edges list.

	//Algorithm for this followed from
	//http://www.cs.cornell.edu/courses/cs6820/2010fa/handouts/approx_algs.pdf  
	int inserted = 0, r = 0;
	for(int i=0; i<edges[1].size(); i++)
	{
		int u = edges[0][i];
		int v = edges[1][i];
		int delta = min(w[u]-s[u], w[v]-s[v]);
		
		s[u] += delta;
		s[v] += delta;
		y[i] += delta;
		
		if(s[u] == w[u])
		{	
			if(S[u] == 0)
			{
				adj_edges[0].push_back(u);
				adj_edges[1].push_back(v);
				inserted = 1;
				S[u] = r+1;
				r++;
			}
		}
		
		if(s[v] == w[v])
		{
			if(S[v] == 0)
			{
				if(inserted != 1)
				{
					adj_edges[0].push_back(u);
					adj_edges[1].push_back(v);
				}
			S[v] = r+1;
			r++;
    		}
		}
		cout<<"\n";
		inserted = 0;
	}

	int final_cost = 0;

	//Printing the vertex cover.
	cout << "vertex cover is " << "\n";
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			if(S[j] == i+1)
			{
				cout << j+1 << "\n";
				final_cost += w[j];
				break;
			}

		}
	}

	//Printing the adjacency edge, add those edges which have become tight
	cout<<"\nAdjacency Edges : ";
	for(size_t i = 0; i < adj_edges[0].size(); ++i) 
	{
		cout << "(" << adj_edges[0][i]+1 << "," << adj_edges[1][i]+1<<")";
	}
	cout<<"\n";

	cout << "cost is " << final_cost << "\n";
} 
