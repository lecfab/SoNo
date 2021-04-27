// See header for general documentation

#include "algo_minuncut.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"
#include "../order/order_deg.h"

using namespace std;


// 4) Take highest deg counting remaining nodes: 409007 -> 258699
// vector<bool> algo_minuncut(Edgelist &hh) {
// Uadjlist g(hh);
vector<bool> algo_minuncut(const Adjlist &g) {
  Debug("Undirected Min Uncut (take highest deg counting remaining nodes)")
  Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n - g.get_degree(u)));

  vector<bool> partition; partition.reserve(g.n);
  vector<bool> placed(g.n, false);
  ul uncut = 0; //, cut = 0;

  for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();
		ul u = kv.key;

    if(!placed[u]) {
      partition[u] = false; // arbitrarily
      placed[u] = true;
    }

    for (auto &v : g.neigh_iter(u)) { // put neighbours in it
      if(!h.contains(v)) continue;
      h.update_increment(v); // neighbours become less important

      if(placed[v] and partition[v] == partition[u])
        uncut ++; // if they are in the same group, increase uncut
      else if(!placed[v]) {
        partition[v] = !partition[u];
        placed[v] = true;
      } // cut ++; // (just to see if total match)
    }
	} // if(uncut + cut != g.e/g.edge_factor) Alert("Edge count not matching: "<< uncut<<"+"<<cut<<"="<<(uncut+cut)<<" / "<<g.e)
  // compute_uncut(g, partition);
  bisection_localsearch(g, partition);
  uncut = compute_uncut(g, partition);
  Info("Min Uncut found: " << uncut)
	return partition;
}

// template<typename Function>
void bisection_localsearch(const Adjlist &g, vector<bool> &partition) { //, Function f) {
  vector<ul> oppositeNeigh(g.n, 0);
  for(ul u=0; u<g.n; ++u) {
    for(auto &v : g.neigh_iter(u)) {
      if(partition[v] != partition[u]) oppositeNeigh[u] ++;
    }
  }

  bool modified = true;
  int c = 0;
  while(modified) {
    c++;
    modified = false;
    for(ul u=0; u<g.n; ++u) { // u = (uv.first + cu*37) % g.n;
      if(g.get_degree(u) > 2*oppositeNeigh[u]) {
        modified = true;
        oppositeNeigh[u] = g.get_degree(u) - oppositeNeigh[u];
        partition[u] = !partition[u];
        for(auto &v : g.neigh_iter(u)) {
          if(partition[v] != partition[u]) oppositeNeigh[v] ++;
          else oppositeNeigh[v] --;
        }
      }
    }
  }
  Debug("Local search took steps "<<c)
}

ul compute_uncut(const Adjlist &g, const vector<bool> &partition) {
  ul uncut=0, cut=0;
  for(ul u=0; u<g.n; ++u) {
    for(auto &v : g.neigh_iter(u)) {
      if(partition[v] != partition[u]) cut ++;
      else uncut++;
    }
  }
  uncut /= 2; cut /= 2;
  Debug("Graph uncut " << uncut << " (cut "<<cut<<", total "<< (cut+uncut) <<")")
  return uncut;
}


// 1) Heap maintaining discovered nodes: 360518 -> 256635
// vector<bool> algo_minuncut(Edgelist &h) {
//   vector<ul> rank_deg = order_deg(h); // degree DESC
//   Uadjlist g(h, rank_deg);
//
//   vector<bool> partition; partition.reserve(g.n);
//   vector<bool> placed(g.n, false);
//   vector<bool> ranked(g.n, false);
// 	bool team = false;
//
//   ul uncut = 0, cut = 0;
//   ul i = 0, inserts=0, pops=0, ranks=0;
//   Bheap heap(g.n + 1);
//   for (ul u = 0; u < g.n; ++u)
//     heap.insert(Keyvalue(u, g.n + 1));
//
//   for (ul u = 0; u < g.n; ++u) {
//     if (placed[u]) // already seen
//       continue;
//
//     heap.update(u, g.n - g.get_degree(u));
//     inserts++;
//     placed[u] = true;
//     partition[u] = team;
//
//     while (inserts > pops) {
//       Keyvalue kv = heap.popmin();
//       pops++;
//       ul w = kv.key;
//       if(ranked[w]){ Info(w<<" ranked") exit(0); }
//       ranked[w] = true; ranks++;
//       team = !partition[w];
//
//       for (auto &v : g.neigh_iter(w)) {
//         if (ranked[v]) continue;
//         if (placed[v]) {
//           if(partition[v] == team) cut ++;
//           else uncut ++;
//           continue;
//         }
//
//         heap.update(v, g.n - g.get_degree(v));
//         inserts++;
//         placed[v] = true;
//         partition[v] = team;
//         cut ++;
//       }
//     }
//   }
//
//   compute_uncut(g, partition);
//   bisection_localsearch(g, partition);
//   compute_uncut(g, partition);
//   return partition;
// }


// 2) BFS with degree-sorted nodes: 363797 -> 253166
// vector<bool> algo_minuncut(Edgelist &h) {
//   vector<ul> rank_deg = order_deg(h); // degree DESC
//   Uadjlist g(h, rank_deg);
//
//
//   vector<bool> partition; partition.reserve(g.n);
//   vector<bool> placed(g.n, false);
// 	bool team = false;
//   ul uncut = 0, cut = 0;
//
//   vector<ul> order; order.reserve(g.n);
//   vector<bool> ranked(g.n, false);
//
//   ul i = 0;
//   for (ul u = 0; u < g.n; ++u) {
//     if (placed[u]) // already seen
//       continue;
//
//     order.push_back(u);
//     placed[u] = true;
//     partition[u] = team;
//
//     while (i < order.size()) {
//       ul w = order[i++];
//       ranked[w] = true;
//       team = !partition[w];
//       for (auto &v : g.neigh_iter(w)) {
//         if(ranked[v]) continue;
//         if (placed[v]) {
//           if(partition[v] == team) cut ++;
//           else uncut ++;
//           continue;
//         }
//
//         order.push_back(v);
//         placed[v] = true;
//         partition[v] = team;
//         cut ++;
//       }
//     }
//   }
//
//   compute_uncut(g, partition);
//   bisection_localsearch(g, partition);
//   compute_uncut(g, partition);
//   return partition;
// }


// 3) BFS starting from highest-degree node: 363797 -> 256557
// vector<bool> algo_minuncut(Edgelist &h) {
//   Uadjlist g(h);
//
//   Debug("Undirected Min Uncut")
//   ul u0 = 0;
// 	for (ul u = 0; u < g.n; ++u)
// 		if(g.get_degree(u) > g.get_degree(u0)) u0 = u;
//
//   vector<bool> partition; partition.reserve(g.n);
//   vector<bool> placed(g.n, false);
// 	bool team = false;
//   ul uncut = 0, cut = 0;
//
//   vector<ul> order; order.reserve(g.n);
//   vector<bool> ranked(g.n, false);
//
//   ul i = 0;
//   for (ul c = 0; c < g.n; ++c) {
//     ul u = (c + u0) % g.n; // allows to start from any node u0
//     if (placed[u]) // already seen
//       continue;
//
//     order.push_back(u);
//     placed[u] = true;
//     partition[u] = team;
//
//     while (i < order.size()) {
//       ul w = order[i++];
//       ranked[w] = true;
//       team = !partition[w];
//       for (auto &v : g.neigh_iter(w)) {
//         if(ranked[v]) continue;
//         if (placed[v]) {
//           if(partition[v] == team) cut ++;
//           else uncut ++;
//           continue;
//         }
//
//         order.push_back(v);
//         placed[v] = true;
//         partition[v] = team;
//         cut ++;
//       }
//     }
//   }
//
//   compute_uncut(g, partition);
//   bisection_localsearch(g, partition);
//   compute_uncut(g, partition);
// 	return partition;
// }
