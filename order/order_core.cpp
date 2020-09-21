#ifndef ORDER_CORE
#define ORDER_CORE

/* warning: only works in undirected graphs */

#include "../utils/adjlist.cpp"
#include "../algo/algo_kcore.cpp"


using namespace std;


vector<ul> order_core(const Adjlist &g) {
	return algo_kcore(g).rank;
}

#endif
