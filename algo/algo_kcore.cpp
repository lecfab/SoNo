/* warning: only works in undirected graphs

space optimisation: remove rank or degeneracy array depending on what you need
			or compact the degeneracy array, remembering only the indices where degeneracy changes
time optimisation: compute both arrays
*/

#include "algo_kcore.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"

using namespace std;

Kdegeneracies algo_kcore(const Adjlist &g) {
  Bheap h(g.n);
  if(g.edge_factor == 1) {
    Info("Directed in-kcore")
    vector<ul> degIn(g.n, 0);
    for (ul u = 0; u < g.n; ++u)
      for (auto &v : g.neigh_iter(u))
        degIn[v] ++;
    for (ul u = 0; u < g.n; ++u)
  		h.insert(Keyvalue(u, degIn[u]));
  }
  else {
    Info("Undirected kcore")
  	for (ul u = 0; u < g.n; ++u)
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
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val<<" / ?")
    for (auto &v : g.neigh_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_kcore(const Badjlist &g) {
  Info("Bidirected kcore")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.get_deg(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val << " / "<<g.get_deg(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_decrement(v);
    for (auto &v : g.neighOut_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_kcoreIn(const Badjlist &g) {
  Info("In-kcore")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.get_degIn(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val << " / "<<g.get_degIn(u))
    for (auto &v : g.neighOut_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_kcoreOut(const Badjlist &g) {
  Info("Out-kcore")
  Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.get_degOut(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val << " / "<<g.get_degOut(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}


Kdegeneracies algo_icore(const Adjlist &g) {
  Bheap h(g.n);
  if(g.edge_factor == 1) {
    Info("Inverted directed core")
    vector<ul> degIn(g.n, 0);
    for (ul u = 0; u < g.n; ++u)
      for (auto &v : g.neigh_iter(u))
        degIn[v] ++;
    for (ul u = 0; u < g.n; ++u)
  		h.insert(Keyvalue(u, g.n-degIn[u]));
  }
  else {
    Info("Inverted undirected core")
  	for (ul u = 0; u < g.n; ++u)
  		h.insert(Keyvalue(u, g.n-g.get_degree(u)));
  }

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
    if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / ?")
    for (auto &v : g.neigh_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_icore(const Badjlist &g) {
  Info("Inverted core")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n-g.get_deg(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
    if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / "<<g.get_deg(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_decrement(v);
    for (auto &v : g.neighOut_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}


Kdegeneracies algo_icoreIn(const Badjlist &g) {
  Info("Inverted in-core")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n - g.get_degIn(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / "<<g.get_degIn(u))
    for (auto &v : g.neighOut_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_icoreOut(const Badjlist &g) {
  Info("Inverted in-core")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n - g.get_degOut(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
    if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / "<<g.get_degOut(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}


vector<ul> algo_kcore_rank(const Adjlist &g) {
	return algo_kcore(g).rank;
}

// Kdegeneracies algo_kcore(const Adjlist &g) {
//   Alert("kcore decomposition should be used with Uadjlist or Badjlist")
//   return Kdegeneracies(0);
// }
// Kdegeneracies algo_kcoreIn(const Adjlist &g) {
//   return algo_kcore(g);
// }
// Kdegeneracies algo_kcoreOut(const Adjlist &g) {
//   return algo_kcore(g);
// }
