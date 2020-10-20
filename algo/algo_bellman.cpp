#include "algo_bellman.h"

#include "../utils/tools.h"
#include "../utils/adjlist.h"

using namespace std;


// fonction Bellman_Ford(G, s)
//   pour chaque sommet u du graphe
//   |        d[u] = +∞
//   d[s] = 0

//   pour k = 1 jusqu'à Nombre de sommets - 1 faire
//    |    pour chaque arc (u, t) du graphe faire
//    |      |    d[t] := min(d[t], d[u] + poids(u, t))

//   retourner d

vector<ul> source_bellman_front(const Adjlist &g, const ul u) {
	vector<ul> dist(g.n, g.n+2); // initialised to infinite distance
	dist[u] = 0;
	vector<ul> updates; updates.reserve(g.n);
	updates.push_back(u);

	for (ul k = 0; k < g.n; ++k) { // stops after Diameter loops
		ul j = 0;
		while(j < updates.size()) { // only check nodes whose distance has been updated
			const ul *v = &updates[j ++];

			for (const ul *w = g.neigh_beg(*v); w < g.neigh_end(*v); ++w) { // for each outgoing edge
				if(dist[*w] > dist[*v]+1) {
					dist[*w] = dist[*v]+1;
					updates.push_back(*w);
				}
			}
		}
		if(updates.size() == 0) break;
		updates.clear();
	}

	return dist;
}

void algo_bellman(const Adjlist &g, ul repeat) {
	Debug("Algo bellman")
	srand (time(NULL));

	while(repeat--) {
		ul u = rand() % g.n;
		vector<ul> dist = source_bellman_front(g, u);

		// ul v=u;
		// for (ul w = 0; w < g.n; ++w)
		// 	if(dist[w] > dist[v] && dist[w] <= g.n)
		// 		v=w;

		//cout << "From "<<u <<"\t furthest " << v << " with distance " << dist[v] << endl;
	}
}

// vector<ul> source_bellman_stack(const Adjlist &g, const ul u) {
// 	vector<ul> dist(g.n, g.n+2); // initialised to infinite distance
// 	dist[u] = 0;
// 	vector<ul> reached; reached.reserve(g.n);
// 	reached.push_back(u);

// 	for (ul k = 0; k < g.n; ++k) { // stops after Diameter loops
// 		bool updated = false;

// 		ul j = 0;
// 		while(j < reached.size()) { // possible improvement: store updated instead of reached
// 			const ul *v = &reached[j ++];

// 			for (const ul *w = g.neigh_beg(*v); w < g.neigh_end(*v); ++w) { // for each edge
// 				if(dist[*w] > dist[*v]+1) {
// 					if(dist[*w] > g.n) reached.push_back(*w);
// 					dist[*w] = dist[*v]+1;
// 					updated = true;
// 				}
// 			}
// 		}
// 		if(!updated) break;
// 	}

// 	return dist;
// }


// vector<ul> source_bellman_directed(const Dadjlist &g, const ul u) {
// 	vector<ul> dist(g.n, g.n+2); // initialised to infinite distance
// 	dist[u] = 0;
// 	for (ul k = 0; k < g.n; ++k) { // do n times but hope it stops early
// 		bool updated = false;
// 		for (ul v=0; v < g.n; ++v) {
// 			if(dist[v] > g.n) continue; // directed
// 			for (ul i = g.cd[v]; i < g.cd[v+1]; ++i) { // for each edge
// 				ul w = g.adj[i];
// 				if(dist[w] > dist[v]+1) {
// 					dist[w] = dist[v]+1;
// 					updated = true;
// 				}
// 			}
// 		}
// 		if(!updated)
// 			break;
// 	}

// 	return dist;
// }

// vector<ul> source_bellman_undirected(const Dadjlist &g, const ul u) {
// 	vector<ul> dist(g.n, g.n+2); // initialised to infinite distance
// 	dist[u] = 0;
// 	for (ul k = 0; k < g.n; ++k) { // do n times
// 		bool updated = false;
// 		for (ul v=0; v < g.n; ++v) {
// 			for (ul i = g.cd[v]; i < g.cd[v+1]; ++i) { // for each edge
// 				ul w = g.adj[i];
// 				if(dist[w] > dist[v]+1) {
// 					dist[w] = dist[v]+1;
// 					updated = true;
// 				}

// 				if(dist[v] > dist[w]+1) { // undirected
// 					dist[v] = dist[w]+1;
// 					updated = true;
// 				}
// 			}
// 		}
// 		if(!updated)
// 			break;
// 	}

// 	return dist;
// }
