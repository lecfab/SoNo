#ifndef ALGO_MINUNCUT_H
#define ALGO_MINUNCUT_H

#include "../utils/tools.h"
struct Adjlist;
struct Edgelist;
struct Bheap;

std::vector<ul> order_minuncut(Edgelist &g);
void bisection_localsearch(const Adjlist &g, std::vector<bool> &partition);
ul compute_uncut(const Adjlist &g, const std::vector<bool> &partition);
// std::vector<bool> algo_minuncut(Edgelist &h);
std::vector<bool> algo_minuncut(Edgelist &g);

#endif
