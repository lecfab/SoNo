#ifndef ALGO_DFS_H
#define ALGO_DFS_H

#include <vector>
#include "../utils/tools.h"
#include "../utils/adjlist.h"
//struct Adjlist;

struct Nodeneigh {
  const ul node;
  const ul *neigh;
  // Nodeneigh(const Adjlist &g, ul &u);
  Nodeneigh(const Adjlist &g, ul &u) : node(u), neigh(g.neigh_beg(u)) {}
};

// time n+m, size n
std::vector<ul> algo_dfs(const Adjlist &g);

// time m+n, size m
// ul source_dfs(const Dadjlist &g, const ul u);

#endif
