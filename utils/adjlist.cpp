/* Structures for compact adjacency lists, built from a list of edges */
#include "adjlist.h"
#include "tools.h"
#include "edgelist.h"
#include <algorithm>

using namespace std;

Adjlist::Adjlist(Edgelist &g, int edge_factor/*=1*/) : edge_factor(edge_factor), n(g.n), e(edge_factor * g.e) {
	cd.reserve(n+1); // warning: .size() will not be defined
	adj.reserve(e);
}

// iterate over neighbours: for (auto v=neigh_beg(u); v<neigh_end(u); ++v)  cout << *v << endl;
ul* Adjlist::neigh_beg(const ul &u) { return &adj[cd[u]]; }
ul* Adjlist::neigh_end(const ul &u) { return &adj[cd[u+1]]; }
const ul* Adjlist::neigh_beg(const ul &u) const { return &adj[cd[u]]; }
const ul* Adjlist::neigh_end(const ul &u) const { return &adj[cd[u+1]]; }
NeighIter Adjlist::neigh_iter(const ul &u) { return NeighIter(neigh_beg(u), neigh_end(u)); }
const NeighIter Adjlist::neigh_iter(const ul &u) const { return NeighIter(neigh_beg(u), neigh_end(u)); }

ul Adjlist::get_degree(const ul &u) const {
	if(u >= n) return 0;
	return cd[u+1] - cd[u];
}

void Adjlist::print_some(int a) {
	cout << "# printing " << a << " edges" << endl;
	for (ul u = 0; u < n; ++u) {
    for (auto &v : neigh_iter(u)) {
			cout << u << " " << v << endl;
			if(a-- < 0) return;
		}
	}
}

void Adjlist::reorder(const vector<ul> &rank) {
	Edgelist h = to_edgelist_ranked(true, rank);
	build_from_edgelist(h, true);
}

Edgelist Adjlist::to_edgelist(bool directed/*=true*/, auto ranker/*=[](ul u) { return u; }*/) {
	Edgelist h(n, directed ? e / edge_factor : e);
	for (ul u = 0; u < n; ++u) {
    for (auto &v : neigh_iter(u))
			if (build_edge_condition(u, v)) // if undirected, put only half of two-sided edges
				h.edges.push_back(edge( ranker(u), ranker(v) ));
		cd[u] = 0;
	}
	return h;
}
Edgelist Adjlist::to_edgelist_ranked(bool directed, const vector<ul> &rank) {
	auto ranker = [&rank](ul u) { return rank[u]; };
	return to_edgelist(directed, ranker);
}


void Adjlist::build_from_edgelist(Edgelist &g, bool sorted/*=false*/) {
	vector<ul> deg = compute_degrees(g);
	compute_cumulated_degrees(deg);
	build_adj_array(g, deg);
	if(sorted) sort_neighbours(); // sort neighbours to respect new order
}

void Adjlist::compute_cumulated_degrees(vector<ul> &deg) {
	cd[0] = 0;
	for (ul u=0; u<n; ++u) {
		cd[u+1] = cd[u] + deg[u];
		deg[u] = 0; // https://stackoverflow.com/questions/8848575/fastest-way-to-reset-every-value-of-stdvectorint-to-0/8849789#8849789
	}
}

void Adjlist::sort_neighbours() {
	for (ul u = 0; u < n; ++u) {
		sort(neigh_beg(u), neigh_end(u));
	}
}



Uadjlist::Uadjlist(Edgelist &g) : Adjlist(g, 2) { build_from_edgelist(g); } // 2*e for undirected

vector<ul> Uadjlist::compute_degrees(Edgelist &g) { // warning: if input file is directed, edges may be counted twice
	vector<ul> deg(n, 0);
	for (ul i=0; i<g.e; ++i) {
		++deg[g.edges[i].first];
		++deg[g.edges[i].second]; // if undirected
	}
	return deg;
}
void Uadjlist::build_adj_array(Edgelist &g, vector<ul> &deg) {
	for (ul i = 0; i<g.e; ++i) {
		ul u = g.edges[i].first;
		ul v = g.edges[i].second;
		adj[ cd[u] + deg[u]++ ] = v;
		adj[ cd[v] + deg[v]++ ] = u; // if undirected
	}
}

Dadjlist::Dadjlist(Edgelist &g) : Adjlist(g, 1) { build_from_edgelist(g); }

vector<ul> Dadjlist::compute_degrees(Edgelist &g) {
	vector<ul> deg(n, 0);
	for (ul i=0; i<g.e; ++i)
		++deg[g.edges[i].first];
	return deg;
}
void Dadjlist::build_adj_array(Edgelist &g, vector<ul> &deg) {
	for (ul i = 0; i<g.e; ++i) {
		ul u = g.edges[i].first;
		adj[ cd[u] + deg[u]++ ] = g.edges[i].second;
	}
}
