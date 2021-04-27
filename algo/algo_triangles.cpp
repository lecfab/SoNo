// See header for general documentation

#include <functional>
#include <unordered_set>
#include <set>
#include "algo_triangles.h"
#include "../utils/adjlist.h"
#include "../utils/edgelist.h"

using namespace std;
// todo: output sum d²

// ull count_triangles_hash(const Badjlist &g) {
//   Info("Counting triangles with hash complexity O(sum min(du+, dv+))")
//
//   ull t = 0;
//   for(ul u=0; u<g.n; ++u) {
//     unordered_set<ul> neighs; neighs.reserve(g.get_degOut(u));
//     for(auto &v : g.neighOut_iter(u))
//       neighs.insert(v);
//
//     for(auto &v : g.neighOut_iter(u)) {
//       for(auto &w : g.neighOut_iter(v))
//         t += neighs.count(w);
//     }
//   }
//   Info("Found triangles: "<< t << "    | millions: " << t/1000000)
//   return t;
// }

void triangle_complexities(const Badjlist &g) {
  ull minpp = 0, minpm = 0, minmp = 0, minmm = 0;
  for(ul u=0; u<g.n; ++u)
    for(auto &v : g.neighOut_iter(u)) {
      minpp += min(g.get_degOut(u), g.get_degOut(v));
      minpm += min(g.get_degOut(u), g.get_degIn(v));
      minmp += min(g.get_degIn(u), g.get_degOut(v));
      minmm += min(g.get_degIn(u), g.get_degIn(v));
    }
  Info("O(sum min(du+, dv+)) : "<< minpp << "    | per edge: " << ((double )minpp)/((double) g.e))
  Info("O(sum min(du+, dv-)) : "<< minpm << "    | per edge: " << ((double )minpm)/((double) g.e))
  Info("O(sum min(du-, dv+)) : "<< minmp << "    | per edge: " << ((double )minmp)/((double) g.e))
  Info("O(sum min(du-, dv-)) : "<< minmm << "    | per edge: " << ((double )minmm)/((double) g.e))

  ull dpp = 0, dpm = 0, dmm = 0, dep = 0;
  for(ul u=0; u<g.n; ++u) {
    dpp += g.get_degOut(u) * g.get_degOut(u);
    dpm += g.get_degOut(u) * g.get_degIn(u);
    dmm += g.get_degIn(u) * g.get_degIn(u);
    dep += g.get_deg(u) * g.get_degOut(u);
  }

  Info("O(sum d+²) : "<< dpp << "    | per edge: " << ((double )dpp)/((double) g.e))
  Info("O(sum d-²) : "<< dmm << "    | per edge: " << ((double )dmm)/((double) g.e))
  Info("O(sum d+d-) : "<< dpm << "    | per edge: " << ((double )dpm)/((double) g.e))
  Info("O(sum d*d+) : "<< dep << "    | per edge: " << ((double )dep)/((double) g.e))
}

ull count_triangles_hash(const Badjlist &g) {
  Info("Counting triangles with hash complexity O(sum min(du+, dv+))")

  unordered_set<edge,pair_hash> edges; edges.reserve(g.e);
  // set<edge> edges;
  // unordered_set<ull> edges;
  for(ul u=0; u<g.n; ++u) {
    // ull offset_u = g.n*u;
    for(auto &v : g.neighOut_iter(u))
      edges.insert({u, v});
      // edges.emplace(edge(u, v));
      // edges.insert(offset_u + v);
  }
  Info("Fillled")
  ull t = 0, c = 0;
  for(ul u=0; u<g.n; ++u) {
    for(auto &v : g.neighOut_iter(u)) {
      if(g.get_degOut(v) < g.get_degOut(u)) {
        // ull offset_u = g.n*u;
        for(auto &w : g.neighOut_iter(v)) {
          t += edges.count({u, w});
          // t += edges.count(edge(u, w));
          // t += edges.count(offset_u + w);
          c++;
        }
      }
      else {
        // ull offset_v = g.n*v;
        for(auto &w : g.neighOut_iter(u)) {
          t += edges.count({v, w});
          // t += edges.count(edge(v, w));
          // t += edges.count(offset_v + w);
          c++;
        }
      }
    }
  }
  Info("Found triangles: "<< t << "    | millions: " << t/1000000)
  Info("Complexity: "<< c << "    | per edge: " << ((double )c)/((double) g.e))
  return t;
}

ull count_triangles_bool(const Badjlist &g, bool uOut_iter, bool vOut_iter) {
  function<NeighIter(ul)> Out_iter = [&g] (ul u) -> NeighIter { return g.neighOut_iter(u); };
  function<NeighIter(ul)> In_iter = [&g] (ul u) -> NeighIter { return g.neighIn_iter(u); };

  auto u_iter = uOut_iter ? Out_iter : In_iter;
  auto v_iter = vOut_iter ? Out_iter : In_iter;

  ull t = 0, c = 0;
  vector<bool> is_neighOut(g.n, false);
  for(ul u=0; u<g.n; ++u) {
    for(auto &v : g.neighOut_iter(u)) is_neighOut[v] = true; // set array

    for(auto &v : u_iter(u)) {
      for(auto &w : v_iter(v)) {
        c ++;
        t += is_neighOut[w];
      }
    }

    for(auto &v : g.neighOut_iter(u)) is_neighOut[v] = false; // reset array
  }
  Info("Found triangles: "<< t << "    | millions: " << t/1000000)
  Info("Complexity: "<< c << "    | per edge: " << ((double )c)/((double) g.e))
  return t;
}

ull count_triangles_dpp(const Badjlist &g) {
  Info("Counting triangles whith complexity O(sum d+²)")
  return count_triangles_bool(g, false, true);
}

ull count_triangles_dmm(const Badjlist &g) {
  Info("Counting triangles whith complexity O(sum d-²)")
  return count_triangles_bool(g, true, false);
}

ull count_triangles_dpm(const Badjlist &g) {
  Info("Counting triangles whith complexity O(sum d+d-)")
  return count_triangles_bool(g, true, true);
}
