#ifndef INOUT
#define INOUT

#include <iostream>
#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files

#include "tools.cpp"

using namespace std;


//compute the maximum of three unsigned long
inline ul max3(ul a, ul b, ul c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the edgelist from file
edgelist readedgelist(ifstream &file) {
	edgelist g;
	g.edges.reserve(NLINKS);
	g.n=0;
	ul u, v;
	while(file >> u >> v) {
		g.edges.push_back(edge(u, v));
		g.n = max3(g.n, u, v);
	}
	g.e = g.edges.size();
	g.n++;
	return g;
}

ul get_number_vertices(ifstream &file) {
	ul n=0, u, v;
	while(file >> u >> v) {
		//Debug(n << ' '<<u <<' '<< v)
		n = max3(n, u, v);
	}
	return n+1;
}

// edgelist c_readedgelist(char* filename) {
// 	FILE *file;
// 	file=fopen(filename,"r");

// 	edgelist g;
// 	g.edges.reserve(NLINKS);
// 	g.n=0;
// 	ul u, v;
// 	while (fscanf(file,"%lu %lu\n", &u, &v)==2) {
// 		g.edges.push_back(edge(u, v));
// 		g.n = max3(g.n, u, v);
// 	}
// 	fclose(file);

// 	g.e = g.edges.size();
// 	g.n++;
// 	return g;
// }

// Experiment: FILE* seems to be 20x faster than ofstream <<
void c_printorder(const vector<ul> &rank, const ul n, char* output){
	FILE *file=fopen(output,"w");
	for (ul i=0; i < n; ++i) { fprintf(file, "%lu\n", rank[i]); }
	fclose(file);
}

// void printorder(const vector<ul> &rank, const ul n, ofstream &file, const Adjlist &g){
// 	for (ul u = 0; u < n; ++u)
// 		file << rank[u] << " " << g.get_degree(u) << endl;
// }


#endif
