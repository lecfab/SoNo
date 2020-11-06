// Reverse Cuthill–McKee algorithm
// https://en.wikipedia.org/wiki/Cuthill-McKee_algorithm

#include <algorithm>
#include "order_rcm.h"
#include "../utils/tools.h"
#include "../utils/edgelist.h"
#include "../utils/adjlist.h"
#include "order_deg.h"
#include "../algo/algo_bfs.h"


using namespace std;


vector<ul> order_rcm(Edgelist &h) {
  vector<ul> rank_deg = order_deg(h, false); // degree ASC
  Dadjlist g(h, rank_deg);
  vector<ul> rank_bfs = rank_from_order(algo_bfs(g), g.n); // Cuthill–McKee

  vector<ul> rank; rank.reserve(h.n);
  for (ul u = 0; u < h.n; u++) {
    rank[u] = h.n-1 - rank_bfs[rank_deg[u]]; // compose reverse Cuthill–McKee with degree
  }
  return rank;
}
