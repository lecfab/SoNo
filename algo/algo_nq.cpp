#include "algo_nq.h"
#include <ctime>
#include <cmath>

#include "../utils/adjlist.h"

ul algo_nq(const Adjlist &g, ul repeat) {
	//cout << "Algo nq" << endl;
	srand (time(NULL));
	ul result = 0;
	while(repeat--) {
		ul u = rand() % g.n;
		//cout << "Neighbours of " << u ;
		ul neigh = 0;
		ul neigh2 = 0;
		for (ul i = g.cd[u]; i < g.cd[u+1]; ++i) {
			ul v = g.adj[i];
			//cout << "\t" << v << endl;
			neigh++;
			neigh2 += pow(g.get_degree(v), 2);
		}
		//cout << " are " << neigh << endl;
		result += neigh2;
	}
	return result;
}
