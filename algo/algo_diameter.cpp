#include "algo_diameter.h"

#include "../utils/tools.h"
#include "../utils/adjlist.h"
#include "algo_bellman.h"

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
