#ifndef ALGO_BELLMAN_H
#define ALGO_BELLMAN_H

#include <vector>
#include "../utils/tools.h"
struct Adjlist;

void algo_bellman(const Adjlist &g, ul repeat);
std::vector<ul> source_bellman_front(const Adjlist &g, const ul u);

#endif
