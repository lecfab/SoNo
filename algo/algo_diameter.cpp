#ifndef ALGO_DIAMETER
#define ALGO_DIAMETER

#include <list>

#include "../utils/tools.cpp"
#include "../utils/adjlist.cpp"
#include "algo_bellman.cpp"

using namespace std;


ul algo_diameter(const Adjlist &g, ul repeat) {
	Debug("Algo diameter")
	srand (time(NULL));

	ul diameter = 0;
	while(repeat--) {
		ul u = rand() % g.n;
		vector<ul> dist = source_bellman_front(g, u);

		ul v=u;
		for (ul w = 0; w < g.n; ++w)
			if(dist[w] > dist[v] && dist[w] <= g.n)
				v=w;

		if(dist[v] > diameter) {
			diameter = dist[v];
			//cout << "Updated diameter "<<diameter <<"\t from " << u << " to " << v << endl;
		}
	}

	return diameter;
}

#endif
