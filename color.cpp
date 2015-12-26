/*
 *	NAME   : col.cpp
 *  AUTHOR : Arjun Krishna
 *  usage  : ./[prog_name] filename N 
 * "filename" contains data in edge List format
 *  N is the number of vertices in the Graph represented in "filename"
 * NOTE : vertices labeled from 0 to N-1 should be in filename 
          and it won't be checked in this code.
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <map>

using namespace std;

// GLOBAL VARIABLES
int N; // N -- Number of Vertices --> argv[2]

ifstream edge;
//
class Graph
{
	// Parameters
	int V;			// no. of vertices
	list <int> * adj;		// adjacency List
	map <int,int> * xCol;	// A List to note what colors can't be used to color a particular vertex
	map <int,int> col;	//vertex number to color used map 
	int colors;	// number od colors used
	int * deg;	//array to store degree
	multimap <int,int> deg_ver; // for sorting degree
	int* ver_order;
	
public:
	Graph(int V,char* fname)    //contructor
	{
		this->V = V;
		adj = new list<int> [V];
		xCol = new map<int,int> [V];
		deg = (int*) malloc(sizeof(int)*V);
		deg_ver.clear();
		for(int i = 0;i<V;i++)
			deg[i] = 0;
		colors = 0;
		readGraph(fname);
		for(int i=0;i<V;i++)
			deg_ver.insert(make_pair(deg[i],i));
		ver_order = (int*) malloc(sizeof(int)*V);
	}
	void addEdge(int a,int b) //Add edge a,b to the Graph
	{
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	void readGraph(char* fname)
	{
		edge.open(fname);
		int a,b;
		while(edge>>a>>b)
		{
			addEdge(a,b);
			(deg[a])++;
			(deg[b])++;
		}
		edge.close();
	}
	void rmVertex(int x,int c) //Remove vertex number x which shall be colored with c 
	{
		while(!adj[x].empty())
		{
			xCol[adj[x].back()].insert(make_pair(c,c));
			adj[x].pop_back();
		}
		col[x] = c;	
	}
	int colMin(int x) // min Color needed to color vertex number x
	{
		map<int,int>::iterator itr;
		if(xCol[x].empty()) return 1;
		else
		{
			int i = 1;
			for(itr = xCol[x].begin() ; itr!=xCol[x].end() ; itr++)
			{
				if((itr->first) != i)
					return i;
				i++;
			}
			return i;	
		}	
	}
	void vertexOrder()
	{
		multimap<int,int>::reverse_iterator itr;
		int i =0;
		for(itr = deg_ver.rbegin(); itr != deg_ver.rend() ; itr++)
			ver_order[i++] = itr->second;
	}
	void colorGraph()	// coloring algorithm
	{
		int c;
		vertexOrder();
		for(int i = 0 ; i<V ; i++)
		{
			c = colMin(ver_order[i]);
			if( c > colors) colors = c;
			rmVertex(ver_order[i],c);
		}		
	}
	void printColoring() //prints the coloring
	{
		map<int,int>::iterator itr;
		cout << "Colors Used : "<<colors <<endl;
		for(itr = col.begin(); itr!= col.end(); itr++)
			cout << itr->first << " " <<itr->second<<endl;
	}
};

/**MAIN**/

int main(int argc,char* argv[])
{
	if(argc!=3) { cerr<<"Usage : ./color fileName N "<<endl;return 0;}
	N = atoi(argv[2]);
	Graph G(N,argv[1]);
	G.colorGraph();
	G.printColoring();
	return 0;
}
