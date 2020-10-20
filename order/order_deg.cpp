#include "order_deg.h"
#include "../utils/adjlist.h"
#include "../utils/edgelist.h"
#include <algorithm>

using namespace std;

inline bool compare_nodedeg (Keyvalue &a, Keyvalue &b) { // sort degree DESC, node ASC
	return ( a.val > b.val or (a.val == b.val and a.key < b.key) );
}


// --------------------------------------------------
// ----------------- From edgelist ------------------
// --------------------------------------------------
vector<ul> rank_from_deg(const vector<ul> &deg, const ul &n) {
	vector<Keyvalue> toSort; toSort.reserve(n);
  for (ul u = 0; u < n; ++u)
    toSort.push_back( Keyvalue(u, deg[u]) );

  sort(toSort.begin(), toSort.end(), compare_nodedeg);

  vector<ul> rank; rank.reserve(n);
  for (ul u = 0; u < n; ++u)
  	rank[ toSort[u].key ] = u;

  return rank;
}

vector<ul> order_deg(Edgelist &g) {
	Debug("Order degOut from edgelist")
  g.compute_degrees();
  return rank_from_deg(g.deg, g.n);
}
vector<ul> order_degOut(Edgelist &g) {
	Debug("Order degOut from edgelist")
  g.compute_degrees();
  return rank_from_deg(g.degOut, g.n);
}

vector<ul> order_degIn(Edgelist &g) {
	Debug("Order degIn from edgelist")
  g.compute_degrees();
  return rank_from_deg(g.degIn, g.n);
}

// --------------------------------------------------
// ----------------- From adjlist -------------------
// --------------------------------------------------
vector<ul> order_degOut(Adjlist &g) {
	Debug("Order degOut")
    vector<Keyvalue> toSort; toSort.reserve(g.n);
    for (ul u = 0; u < g.n; ++u)
        toSort.push_back( Keyvalue(u, g.get_degree(u)) );

    // cout << "before sort: "<< endl; for (ul i = 0; i < 10; ++i) cout << "\t" << toSort[i].node << " \t " << toSort[i].deg << endl;
    sort(toSort.begin(), toSort.end(), compare_nodedeg);

    vector<ul> rank; rank.reserve(g.n);
    for (ul u = 0; u < g.n; ++u)
    	rank[ toSort[u].key ] = u;

    // cout << "after sort: " << endl; for (ul i = 0; i < 10; ++i) cout << "\t" << toSort[i].node << " \t " << toSort[i].deg << " test:" << rank[i] << endl;
    return rank;
}

vector<ul> order_degIn(Adjlist &g) {
	Debug("Order degIn")
	vector<Keyvalue> toSort; toSort.reserve(g.n);
	for (ul u = 0; u < g.n; ++u)
	    toSort.push_back( Keyvalue(u, 0) );

	for (ul i = 0; i < g.e; ++i)
		toSort[g.adj[i]].val ++;

	sort(toSort.begin(), toSort.end(), compare_nodedeg);

	vector<ul> rank; rank.reserve(g.n);
	for (ul u = 0; u < g.n; ++u)
		rank[ toSort[u].key ] = u;
	return rank;
}
