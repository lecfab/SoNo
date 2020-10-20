#include "algo_tarjan.h"
#include "../utils/adjlist.h"
#include "algo_dfs.h"

using namespace std;


// algo_tarjan ( graph G ) without recursion
//   DFSstack, SCCstack, lowlink
//   indices (n, infinity)
//   foundSCC (n, false)
//
//   for w in V
//     if indices[w] < infinity, continue
//     push w to DFSstack
//
//     while DFSstack is not empty
//       let u be the top of DFSstack
//
//       if indices[u] is infinity
//         define indicies[u] = lowlink[u] = max(indices)+1
//         push u to SCCstack
//       else
//         lowlink[u] = min(lowlink[u], backtrack)
//
//       if u has a neighbour v
//         if indices[v] is infinity
//           push v to DFSstack
//         else if not foundSCC[v]
//           lowlink[u] = min(lowlink[u], indices[v])
//
//       else (u has no more neighbour)
//         if lowlink[u] is index[u]
//           create a new SCC
//           for each v on SCCstack until u
//             put v on the SCC
//             foundSCC[v] is true

vector<ul> algo_tarjan(const Adjlist &g) {
  vector<Nodeneigh> DFSstack; DFSstack.reserve(g.n);
  vector<ul> SCCstack; SCCstack.reserve(g.n);
  vector<ul> indices(g.n, g.n + 2);
  vector<ul> lowlink; lowlink.reserve(g.n);
  vector<bool> foundSCC(g.n); foundSCC.reserve(g.n);
  vector<ul> SCCsizes;

  ul index = 0;
  ul backtrack = g.n + 2;

  for(ul root=0; root<g.n; ++root) {
    if(indices[root] < g.n) continue; // u was already found
    Debug("Start new exploration from " << root)

    DFSstack.push_back(Nodeneigh(g, root));

    while (!DFSstack.empty()) {
      Nodeneigh &deepest = DFSstack.back();
      ul u = deepest.node;
      if(indices[u] > g.n) { // node seen for the first time, added to the SCC stack
        Debug("Discover " << u << ", index " << index)
        indices[u] = index++;
        lowlink[u] = indices[u];
        backtrack = indices[u];
        SCCstack.push_back(u);
        foundSCC[u] = false;
      }
      else { // node seen after seing its neighbours, update accordingly
        Debug("Meet again " << u << ", lowlink (" << lowlink[u] << " vs " << backtrack << ")")
        lowlink[u] = min(lowlink[u], backtrack);
      }

      if(deepest.neigh >= g.neigh_end(u)) { // u has no more neighbour
        Debug("\tNo more neighbour, backtracks " << lowlink[u])
        backtrack = lowlink[u];

        if(lowlink[u] == indices[u]) {
          Debug("\tCreate SCC " << SCCsizes.size())
          ul size = 0, v;
          do {
            Debug("\t\t" << v << " added to SCC " << indices[v])
            v = SCCstack.back();
            indices[v] = SCCsizes.size();
            foundSCC[v] = true;
            size ++;
            SCCstack.pop_back();
          } while(v != u);
          SCCsizes.push_back(size);
        }

        DFSstack.pop_back();
      }
      else {
        ul v = *deepest.neigh;
        deepest.neigh ++;

        if(indices[v] > g.n) { // this neighbour has never been seen
            Debug("\tStack " << v)
            DFSstack.push_back(Nodeneigh(g, v));
        }
        else if(!foundSCC[v]) { // this neighbour has no SCC yet
          Debug("\tneighbour " << v <<" of index "<< indices[v] << " vs lowlink " << lowlink[u] << " (" << lowlink[v] <<")")
          lowlink[u] = min(lowlink[u], indices[v]);
        }
        else {
          Debug("\tFound neighbour " << v << " with SCC " << indices[v])
        }
      }
    }
  }

  ul biggestIndex = 0, biggestSize = 0;
  for (size_t i = 0; i < SCCsizes.size(); i++) {
    if(SCCsizes[i] > biggestSize) {
      biggestIndex = i;
      biggestSize = SCCsizes[i];
    }
    //cout << "SCC " << i << " has size " << SCCsizes[i] << endl;
  }

  return indices;
}
