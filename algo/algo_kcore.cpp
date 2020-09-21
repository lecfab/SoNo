#ifndef ALGO_KCORE
#define ALGO_KCORE

/* warning: only works in undirected graphs

space optimisation: remove rank or degeneracy array depending on what you need
			or compact the degeneracy array, remembering only the indices where degeneracy changes
time optimisation: compute both arrays
*/

#include "../utils/adjlist.cpp"
#include "../utils/heap.cpp"


using namespace std;


struct Kdegeneracies {
	ul n;
	vector<ul> degeneracies;
	vector<ul> rank;
	Kdegeneracies(ul n) : n(n) { degeneracies.reserve(n); rank.reserve(n); }
};

Kdegeneracies algo_kcore(const Adjlist &g) {
	vector<ul> rank; rank.reserve(g.n);

	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u) {
		h.insert(Keyvalue(u, g.get_degree(u)));
	}

	Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		//cout << "round "<<i <<endl; for (ul u = 0; u < g.n; ++u) cout << "\t" << u << ":" << h.key_to_val(u) << endl;
		Keyvalue kv = h.popmin();
		//cout << "POPMIN " << kv.key << ":" << kv.val << endl;
		
		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
		for (auto v = g.neigh_beg(u); v < g.neigh_end(u); ++v) {
			h.update(*v, h.key_to_val(*v)-1);
		}
	}

	return kd;
}

#endif
