#ifndef ORDER_DEG
#define ORDER_DEG

#include <algorithm>

//#include "tools.cpp"
#include "../utils/adjlist.cpp"

struct Nodedeg {
	ul node;
	ul deg;
	Nodedeg(ul node, ul deg) : node(node), deg(deg) {}
	//inline bool operator<=(const X& lhs, const X& rhs){ return !(lhs > rhs); }
};

inline bool compare_nodedeg (Nodedeg &a, Nodedeg &b) { // sort degree DESC, node ASC
	return ( a.deg > b.deg or (a.deg == b.deg and a.node < b.node) );
}


// --------------------------------------------------
// ----------------- From edgelist ------------------
// --------------------------------------------------
vector<ul> rank_from_deg(const vector<ul> &deg, const ul &n) {
	vector<Nodedeg> toSort; toSort.reserve(n);
    for (ul u = 0; u < n; ++u)
        toSort.push_back( Nodedeg(u, deg[u]) );

    sort(toSort.begin(), toSort.end(), compare_nodedeg);
    
    vector<ul> rank; rank.reserve(n);
    for (ul u = 0; u < n; ++u)
    	rank[ toSort[u].node ] = u;

    return rank;
}

vector<ul> order_deg(edgelist &g) {
	Debug("Order deg from edgelist")
	vector<ul> deg; deg.reserve(g.n);
	for (ul i=0; i<g.e; ++i) {
		++deg[g.edges[i].first];
		++deg[g.edges[i].second];
	}

    return rank_from_deg(deg, g.n);
}

vector<ul> order_degOut(edgelist &g) {
	Debug("Order degOut from edgelist")
	vector<ul> deg; deg.reserve(g.n);
	for (ul i=0; i<g.e; ++i)
		++deg[g.edges[i].first];

    return rank_from_deg(deg, g.n);
}

vector<ul> order_degIn(edgelist &g) {
	Debug("Order degIn from edgelist")
	vector<ul> deg; deg.reserve(g.n);
	for (ul i=0; i<g.e; ++i)
		++deg[g.edges[i].second];

    return rank_from_deg(deg, g.n);
}

// --------------------------------------------------
// ----------------- From adjlist -------------------
// --------------------------------------------------
vector<ul> order_degOut(Adjlist &g) {
	Debug("Order degOut")
    vector<Nodedeg> toSort; toSort.reserve(g.n);
    for (ul u = 0; u < g.n; ++u)
        toSort.push_back( Nodedeg(u, g.get_degree(u)) );

    // cout << "before sort: "<< endl; for (ul i = 0; i < 10; ++i) cout << "\t" << toSort[i].node << " \t " << toSort[i].deg << endl;
    sort(toSort.begin(), toSort.end(), compare_nodedeg);
    
    vector<ul> rank; rank.reserve(g.n);
    for (ul u = 0; u < g.n; ++u)
    	rank[ toSort[u].node ] = u;

    // cout << "after sort: " << endl; for (ul i = 0; i < 10; ++i) cout << "\t" << toSort[i].node << " \t " << toSort[i].deg << " test:" << rank[i] << endl;
    return rank;
}

vector<ul> order_degIn(Adjlist &g) {
	Debug("Order degIn")
	vector<Nodedeg> toSort; toSort.reserve(g.n);
	for (ul u = 0; u < g.n; ++u)
	    toSort.push_back( Nodedeg(u, 0) );

	for (ul i = 0; i < g.e; ++i)
		toSort[g.adj[i]].deg ++;

	sort(toSort.begin(), toSort.end(), compare_nodedeg);

	vector<ul> rank; rank.reserve(g.n);
	for (ul u = 0; u < g.n; ++u)
		rank[ toSort[u].node ] = u;
	return rank;
}

#endif
