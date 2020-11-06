#include "algo_bfs.h"
#include <ctime>
#include <vector>
#include "../utils/adjlist.h"
#include "../utils/tools.h"

using namespace std;


vector<ul> algo_bfs(const Adjlist &g) {
  Debug("Algo bfs")
  vector<bool> placed(g.n, false);
  vector<ul> order;
  order.reserve(g.n); // warning: this is an inverted rank

  ul i = 0;
  for (ul u = 0; u < g.n; ++u) {
    if (placed[u])
      continue;

    order.push_back(u);
    placed[u] = true;

    while (i < order.size()) {
      ul w = order[i++];
      for (auto &v : g.neigh_iter(w)) {
        if (placed[v])
          continue;
        placed[v] = true;
        order.push_back(v);
      }
    }
  }
  if(g.n != order.size()) Alert("BFS found only "<<order.size()<<" out of "<<g.n)

  return order;
}
