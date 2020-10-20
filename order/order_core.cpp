/* warning: only works in undirected graphs */

#include "order_core.h"
#include "../utils/adjlist.h"
#include "../algo/algo_kcore.h"

using namespace std;


vector<ul> order_core(const Adjlist &g) {
	// return algo_kcore(g).rank;
  return algo_kcore_rank(g);
}
