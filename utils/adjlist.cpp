#ifndef ADJLIST
#define ADJLIST

/* Structures for compact adjacency lists, built from a list of edges */
#include <vector>
#include <tuple>
#include <list>
#include <algorithm>

#include "tools.cpp"

using namespace std;

struct Adjlist {
public:
	int edge_factor;
	ul n;//number of nodes
	ul e;//number of edges
	vector<ul> cd;//cumulative degree cd[0]=0, length=n+1
	vector<ul> adj;//concatenated lists of neighbors of all nodes, length e

	Adjlist(edgelist &g, int edge_factor=1) : edge_factor(edge_factor), n(g.n), e(edge_factor * g.e) {
		cd.reserve(n+1); // warning: .size() will not be defined
		adj.reserve(e);
	}

	// iterate over neighbours: for (auto v=neigh_beg(u); v<neigh_end(u); ++v)  cout << *v << endl;
	ul* neigh_beg(const ul &u) { return &adj[cd[u]]; }
	ul* neigh_end(const ul &u) { return &adj[cd[u+1]]; }
	const ul* neigh_beg(const ul &u) const { return &adj[cd[u]]; }
	const ul* neigh_end(const ul &u) const { return &adj[cd[u+1]]; }

	ul get_degree(const ul &u) const {
		if(u >= n) return -1;
		return cd[u+1] - cd[u];
	}

	void print_some(int a) {
		cout << "# printing " << a << " edges" << endl;
		for (ul u = 0; u < n; ++u) {
			for (auto v=neigh_beg(u); v<neigh_end(u); ++v) {
				cout << u << " " << *v << endl;
				if(a-- < 0) return;
			}
		}
	}

	void reorder(const vector<ul> &rank) {
		edgelist h = to_edgelist_ranked(true, rank);
		build_from_edgelist(h, true);
	}

	edgelist to_edgelist(bool directed=true, auto ranker=[](ul u) { return u; }) {
		edgelist h;
		h.n = n;
		h.e = directed ? e / edge_factor : e;
		h.edges.reserve(h.e);
		for (ul u = 0; u < n; ++u) {
			for (auto v=neigh_beg(u); v<neigh_end(u); ++v)
				if (build_edge_condition(u, *v)) // if undirected, put only half of two-sided edges
					h.edges.push_back(edge( ranker(u), ranker(*v) ));
			cd[u] = 0;
		}
		return h;
	}
	edgelist to_edgelist_ranked(bool directed, const vector<ul> &rank) {
		auto ranker = [&rank](ul u) { return rank[u]; };
		return to_edgelist(directed, ranker);
	}

	
protected: // Private building operations
	void build_from_edgelist(edgelist &g, bool sorted=false) {
		vector<ul> deg = compute_degrees(g);
		compute_cumulated_degrees(deg);
		build_adj_array(g, deg);
		if(sorted) sort_neighbours(); // sort neighbours to respect new order
	}

	virtual vector<ul> compute_degrees(edgelist &g) {}
	virtual void build_adj_array(edgelist &g, vector<ul> &deg) {}
	virtual inline bool build_edge_condition(const ul &u, const ul &v) {}

	void compute_cumulated_degrees(vector<ul> &deg) {
		cd[0] = 0;
		for (ul u=0; u<n; ++u) {
			cd[u+1] = cd[u] + deg[u];
			deg[u] = 0; // https://stackoverflow.com/questions/8848575/fastest-way-to-reset-every-value-of-stdvectorint-to-0/8849789#8849789
		}
	}

	void sort_neighbours() {
		for (ul u = 0; u < n; ++u) {
			sort(neigh_beg(u), neigh_end(u));
		}
	}
};

struct Uadjlist : Adjlist {
	Uadjlist(edgelist &g) : Adjlist(g, 2) { build_from_edgelist(g); } // 2*e for undirected

	vector<ul> compute_degrees(edgelist &g) { // warning: if input file is directed, edges may be counted twice
		vector<ul> deg(n, 0);
		for (ul i=0; i<g.e; ++i) {
			++deg[g.edges[i].first];
			++deg[g.edges[i].second]; // if undirected
		}
		return deg;
	}
	
	void build_adj_array(edgelist &g, vector<ul> &deg) {
		for (ul i = 0; i<g.e; ++i) {
			ul u = g.edges[i].first;
			ul v = g.edges[i].second;
			adj[ cd[u] + deg[u]++ ] = v;
			adj[ cd[v] + deg[v]++ ] = u; // if undirected		
		}
	}

	inline bool build_edge_condition(const ul &u, const ul &v) { return u < v; }
};

struct Dadjlist : Adjlist {
	Dadjlist(edgelist &g) : Adjlist(g, 1) { build_from_edgelist(g); }

	vector<ul> compute_degrees(edgelist &g) {
		vector<ul> deg(n, 0);
		for (ul i=0; i<g.e; ++i)
			++deg[g.edges[i].first];
		return deg;
	}
	
	void build_adj_array(edgelist &g, vector<ul> &deg) {
		for (ul i = 0; i<g.e; ++i) {
			ul u = g.edges[i].first;
			adj[ cd[u] + deg[u]++ ] = g.edges[i].second;
		}
	}

	inline bool build_edge_condition(const ul &u, const ul &v) { return true; }
};

#endif
