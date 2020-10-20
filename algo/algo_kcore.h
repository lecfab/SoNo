#ifndef ALGO_KCORE_H
#define ALGO_KCORE_H

/* warning: only works in undirected graphs

space optimisation: remove rank or degeneracy array depending on what you need
                        or compact the degeneracy array, remembering only the
indices where degeneracy changes time optimisation: compute both arrays
*/

#include "../utils/tools.h"
#include <vector>
struct Adjlist;

struct Kdegeneracies {
  ul n;
  std::vector<ul> degeneracies;
  std::vector<ul> rank;
  // Kdegeneracies(ul n);
  Kdegeneracies(ul n) : n(n) {
    degeneracies.reserve(n);
    rank.reserve(n);
  }
};

Kdegeneracies algo_kcore(const Adjlist &g);
std::vector<ul> algo_kcore_rank(const Adjlist &g);

#endif
