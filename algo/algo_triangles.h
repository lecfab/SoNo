#ifndef ALGO_TRIANGLES_H
#define ALGO_TRIANGLES_H

#include "../utils/tools.h"
struct Badjlist;

struct pair_hash
{
  template <class T1, class T2>
  std::size_t operator() (const std::pair<T1, T2> &pair) const {
    auto h1 = std::hash<T1>()(pair.first);
    auto h2 = std::hash<T2>()(pair.second);
    h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    return h1;
  }
};

void triangle_complexities(const Badjlist &g);


ull count_triangles_hash(const Badjlist &g);

ull count_triangles_bool(const Badjlist &g, bool uOut_iter, bool vOut_iter);
ull count_triangles_dpp(const Badjlist &g);
ull count_triangles_dmm(const Badjlist &g);
ull count_triangles_dpm(const Badjlist &g);



#endif
