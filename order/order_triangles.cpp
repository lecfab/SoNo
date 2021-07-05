// See header for general documentation

#include <functional>
#include <unordered_set>
#include <set>
#include "order_triangles.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"

using namespace std;




// mistakes: pi should have double type in neighbour_positions
// mistakes: end value should be pi+1 and not pointer+1
// improved: watchlist of updated nodes
// improved: linear computing of best penalties
// improved: linked list instead of array where everyone had to be moved one by one
// improved: when u goes after v, put u close to v instead of at the end of the chain (normalisation and cache purpose)
// todo: watch heap instead of bool-array, but it seems slower in practice
// todo: altitude to be evened out among several predecessors
vector<ul> place_neighbour_dpp(const Adjlist &g) { return place_neighbour(g, sum_square_dpp, best_position_dpp); }
vector<ul> place_neighbour_dpm(const Adjlist &g) { return place_neighbour(g, sum_square_dpm, best_position_dpm); }
vector<ul> place_neighbour(const Adjlist &g, decltype(sum_square_dpm) &scoring, decltype(best_position_dpm) &best_position) {
  ul m = g.e / g.edge_factor;
  // ----- preparation -----
  vector<double> altitude; altitude.reserve(g.n);
  list<ul> chain;
  vector<list<ul>::iterator> pointer; pointer.reserve(g.n);
  // unordered_set<ul> next_nodes; next_nodes.reserve(g.n);
  for(ul u=0; u<g.n; ++u) {
    altitude.push_back(u-((double) g.n) / 2);
    pointer.push_back(chain.insert(chain.end(), u));
    // next_nodes.insert(u);
  }

  vector<ul> dp = compute_dp(g, altitude);
  vector<bool> watch(g.n, true);

  // ----- loop until stability is reached -----
  ul score = scoring(g, dp), previous_score = score * 2;
  cout << "Initial score: " << ((double) score) / m << endl;
  double precision = 1*1e-3; // SET PRECISION
  while(score < previous_score) {
    if(score >= (1.-precision) * previous_score) {
      Info("Truncated with precision "<< precision)
      break;
    }
    previous_score = score;

    // select nodes that have been updated
    vector<ul> nodes; nodes.reserve(g.n);
    // for(auto &u : next_nodes) nodes.push_back(u);
    // next_nodes = {};
    for(ul u=0; u<g.n; ++u)
      if(watch[u]) {
        watch[u] = false;
        nodes.push_back(u);
      }

    // ----- consider each node in random order -----
    random_shuffle(nodes.begin(), nodes.end());
    for(auto &u : nodes) {
      vector<ul> neighbours; neighbours.reserve(g.get_degree(u));
      for(auto &v : g.neigh_iter(u)) neighbours.push_back(v);
      sort(neighbours.begin(), neighbours.end(), [&altitude](ul v, ul w) { return altitude[v] < altitude[w]; });

      // ----- find the best position in its neighbourhood -----
      ul i0 = g.get_degree(u) - dp[u];
      auto pos_penalty = best_position(g, u, neighbours, dp, i0);
      if(pos_penalty.second == 0) continue;
      score += pos_penalty.second;

      // ----- put it in the best position: update out-degrees and altitude -----
      dp[u] = g.get_degree(u) - pos_penalty.first;
      for(ul j=pos_penalty.first; j<i0; ++j) dp[ neighbours[j] ] --;
      for(ul j=i0; j<pos_penalty.first; ++j) dp[ neighbours[j] ] ++;
      // next_nodes.insert(u);
      for(auto &v : neighbours) watch[v] = true; // next_nodes.insert(v);

      auto pointer_next = next(pointer[neighbours.back()]);
      if(pos_penalty.first < g.get_degree(u)) { pointer_next = pointer[ neighbours[pos_penalty.first] ]; }
      pointer[u] = insert_into_chain(chain, pointer[u], pointer_next, altitude);
    }

    cout << "Current score: " << ((double) score) / m << "\t ("<<100*((double) nodes.size()) / g.n<<"% nodes)" <<endl;
  }

  // just to check that score is accurate
  // dp = compute_dp(g, altitude);
  // score = scoring(g, dp);
  cout << "Final score:   " << ((double) score) / m << endl;

  return rank_from_chain(chain);
}




ul sum_square_dpp(const Adjlist &g, const vector<ul> &vec) {
  ul s = 0;
  for(auto &a : vec)
    s += pow(a, 2);
  return s;
}
ul sum_square_dpm(const Adjlist &g, const vector<ul> &dp) {
  ul s = 0;
  for(ul u=0; u<g.n; ++u)
    s += dp[u] * (g.get_degree(u) - dp[u]);
  return s;
}
// ul sum_mindpp(const Adjlist &g, const vector<ul> &dp) {
//   ul s = 0;
//   for(ul u=0; u<g.n; ++u)
//     for(auto &v : g.neigh_iter(u))
//       if(altitude[v] > altitude[u])
//         s += min(dp[u], dp[v]);
//   return s;
// }



pair<ul, long long int> best_position_dpp(const Adjlist &g, const ul &u, const vector<ul> &neighbours, const vector<ul> &dp, ul i0) {
  pair<ul, long long int> pos_penalty = { 0, 0 };
  long long int neighbour_penalty = 0;
  for(ul i=i0; i>0; --i) {
    ul j = i-1;
    neighbour_penalty += 1 - 2*dp[neighbours[j]];
    long long int penalty = neighbour_penalty + j*j - i0*i0 + 2*(i0-j)*g.get_degree(u);
    if(penalty < pos_penalty.second) pos_penalty = { j, penalty };
  }
  neighbour_penalty = 0;
  for(ul i=i0; i<g.get_degree(u); ++i) {
    ul j = i+1;
    neighbour_penalty += 1 + 2*dp[neighbours[i]];
    long long int penalty = neighbour_penalty + j*j - i0*i0 + 2*(i0-j)*g.get_degree(u);
    if(penalty < pos_penalty.second) pos_penalty = { j, penalty };
  }
  return pos_penalty;
}

pair<ul, long long int> best_position_dpm(const Adjlist &g, const ul &u, const vector<ul> &neighbours, const vector<ul> &dp, ul i0) {
  pair<ul, long long int> pos_penalty = { 0, 0 };
  long long int neighbour_penalty = 0;
  for(ul i=i0; i>0; --i) {
    ul j = i-1;
    neighbour_penalty += 2*dp[neighbours[j]] - g.get_degree(neighbours[j]) - 1;
    long long int penalty = neighbour_penalty + i0*i0 - j*j + (j-i0)*g.get_degree(u);
    if(penalty < pos_penalty.second) pos_penalty = { j, penalty };
  }
  neighbour_penalty = 0;
  for(ul i=i0; i<g.get_degree(u); ++i) {
    ul j = i+1;
    neighbour_penalty += g.get_degree(neighbours[i]) - 2*dp[neighbours[i]] - 1;
    long long int penalty = neighbour_penalty + i0*i0 - j*j + (j-i0)*g.get_degree(u);
    if(penalty < pos_penalty.second) pos_penalty = { j, penalty };
  }
  return pos_penalty;
}

list<ul>::iterator insert_into_chain(list<ul> &chain, list<ul>::iterator pointer_u, list<ul>::iterator pointer_next, vector<double> &altitude) {
  chain.erase(pointer_u);
  double altitude_prev = altitude[chain.front()] - 1;
  double altitude_next = altitude[chain.back()] + 1;
  if(pointer_next != chain.end()) altitude_next = altitude[*pointer_next];
  if(pointer_next != chain.begin()) altitude_prev = altitude[*prev(pointer_next)];
  pointer_u = chain.insert(pointer_next, *pointer_u);

  altitude[*pointer_u] = (altitude_next + altitude_prev) / 2;
  if(altitude_prev >= altitude[*pointer_u] or altitude[*pointer_u] >= altitude_next) { // small float limit
    Info("Normalising altitude") // could we normalise only where it is useful?
    double counter = -((double) altitude.size()) / 2;
    for(auto &w : chain) altitude[w] = counter++;
  }

  return pointer_u;
}

vector<ul> compute_dp(const Adjlist &g, const vector<double> &altitude) {
  vector<ul> dp(g.n, 0);
  for(ul u=0; u<g.n; ++u)
    for(auto &v : g.neigh_iter(u))
      if(altitude[v] > altitude[u]) dp[u] ++;
  return dp;
}
vector<ul> rank_from_chain(const list<ul> &chain) {
  vector<ul> rank; rank.reserve(chain.size());
  ul counter = 0;
  for(auto &w : chain) rank[w] = counter++;
  return rank;
}
