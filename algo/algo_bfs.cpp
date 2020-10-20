#include "algo_bfs.h"
#include <ctime>
#include <vector>
#include "../utils/adjlist.h"
#include "../utils/tools.h"

using namespace std;


vector<ul> algo_bfs(const Adjlist &g) {
  Debug("Algo bfs") vector<bool> placed(g.n, false);
  vector<ul> rank;
  rank.reserve(g.n); // warning: this is an inverted rank

  ul i = 0;
  for (ul u = 0; u < g.n; ++u) {
    if (placed[u])
      continue;

    rank.push_back(u);
    placed[u] = true;

    while (i < rank.size()) {
      u = rank[i++];
      for (auto v = g.neigh_beg(u); v < g.neigh_end(u); ++v) {
        if (placed[*v])
          continue;
        placed[*v] = true;
        rank.push_back(*v);
      }
    }
  }

  return rank;
}
