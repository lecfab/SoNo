/* warning: only works in undirected graphs

space optimisation: remove rank or degeneracy array depending on what you need
			or compact the degeneracy array, remembering only the indices where degeneracy changes
time optimisation: compute both arrays
*/

#include "algo_kcore.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"

using namespace std;


vector<ul> algo_kcore_rank(const Adjlist &g) {
  Bheap h(g.n);
  for (ul u = 0; u < g.n; ++u)
    h.insert(Keyvalue(u, g.get_degree(u)));

  vector<ul> rank; rank.reserve(g.n);

  ul degeneracy = 0;
  for (ul i = 1; i <= g.n; ++i) {
    Keyvalue kv = h.popmin();
    if(kv.val > degeneracy)	degeneracy = kv.val;
    rank[kv.key] = g.n - i;
    for (auto &v : g.neigh_iter(kv.key))
      h.update_decrement(v); // h.update(v, h.key_to_val(v)-1);
  }

  Info("Graph degeneracy " << degeneracy)
	return rank;
}

Kdegeneracies algo_kcore(const Adjlist &g) {
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u) {
		h.insert(Keyvalue(u, g.get_degree(u)));
	}

	Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    Debug(u << " has degeneracy " << degeneracy << " and rank " << g.n-i)
		for (auto &v : g.neigh_iter(u)) {
			h.update_decrement(v); // h.update(v, h.key_to_val(v)-1);
		}
	}

	return kd;
}
