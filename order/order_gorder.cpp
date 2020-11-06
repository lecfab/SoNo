#include <vector>

#include "order_gorder.h"
#include "../utils/edgelist.h"
#include "../utils/adjlist.h"
#include "../utils/tools.h"
#include "../utils/unitheap.h"
#include "order_rcm.h"

using namespace std;

vector<ul> complete_gorder(Edgelist &h, ul window) {
  Info("Applying RCM order");
  vector<ul> rank_rcm = order_rcm(h);
  // for(int i=0;i<100&&i<h.n;i++) cout << i << " ranks " << rank_rcm[i] << endl;
  Info("Converting to bidirected adjacency list");
  Badjlist g(h, rank_rcm);
  // g.print_some(100);

  Info("Applying Gorder");
  vector<ul> rank_gorder = order_gorder(g, window);

  // Edgelist hh = g.to_edgelist_ranked(true, rank_gorder);
  // hh.print_some(100);

  vector<ul> rank; rank.reserve(g.n);
  for (ul u = 0; u < g.n; u++) {
    rank[u] = rank_gorder[rank_rcm[u]];
  }
  return rank;
}



vector<ul> order_gorder(const Badjlist &g, ul window) {
  vector<ul> order; order.reserve(g.n);
  UnitHeap heap(g.n);

  ul order_pointer = g.n-1;
  for (ul u = 0; u < g.n; ++u) {
    // I think heap can deal with isolated nodes with no special intervention
    // if(g.get_deg(u) == 0) {// put isolated nodes at the end
    //   Debug(u << " isolated with rank " << order_pointer)
    //   order[order_pointer--] = u;
    // }
    // else
      heap.InsertElement(u, g.get_degIn(u));
  }

  heap.ReConstruct(); // heap indegree DESC sort

  // ul tmp_deg = 0, tmp_u=heap.none;
  // for (ul u = 0; u < g.n; ++u) {
  //   if(g.get_deg(u) > tmp_deg) { tmp_deg = g.get_deg(u); tmp_u=u;}
  //   else if(g.get_deg(u) == 0) {
  //     order[order_pointer--] = u;
  //     heap.DeleteElement(u);
  //   }
  // }
  //
  // ul hub = tmp_u;
  ul hub = heap.top;
  order_pointer = 0;
  order[order_pointer++] = hub;

  heap.DeleteElement(hub);

  move_window(g, heap, hub, hub);

  while(heap.heapsize > 0) {
    Debug("extract")
    ul new_node = heap.ExtractMax();
    Debug(new_node << " with rank " << order_pointer)
    order[order_pointer++] = new_node;

    ul old_node = new_node;
    if(order_pointer >= window+1) // remove oldest node of window
      old_node = order[order_pointer-window-1];
    move_window(g, heap, new_node, old_node);
  }

  // vector<ul> rank; rank.reserve(g.n);
  // for (ul i = 0; i < g.n; ++i)
  //   rank[ order[i] ] = i;
  // return rank;
  Debug("Gorder done")
  while(order_pointer<g.n) Alert(order[order_pointer] <<" remains with rank "<<order_pointer++)
  return rank_from_order(order, g.n);
}

void move_window(const Badjlist &g, UnitHeap &heap, const ul &new_node, const ul &old_node) {
  auto old_parent = g.neighIn_beg(old_node);
  auto new_parent = g.neighIn_beg(new_node);

  if(old_node == new_node) // trick for the case where there is no old_node
    old_parent = g.neighIn_end(old_node); // put directly at the END to ignore
  else {
    if(g.get_degOut(old_node) <= heap.huge)
      for(auto child : g.neighOut_iter(old_node))
        heap.lazyIncrement(child, -locality_child(g, old_node, child));
  }

  if(g.get_degOut(new_node) <= heap.huge)
    for(auto child : g.neighOut_iter(new_node))
      heap.lazyIncrement(child, +locality_child(g, new_node, child));

  while(true) { // parents: increase new, decrease old, ignore common
    int factor = -1;
    if(old_parent >= g.neighIn_end(old_node)) {
      if(new_parent >= g.neighIn_end(new_node)) break; // no parents remain
      factor = 1;
    }
    else if(new_parent < g.neighIn_end(new_node)) {
      if(*new_parent == *old_parent) { // common parents should be ignored
        old_parent++;
        new_parent++;
        continue;
      }
      if(*new_parent < *old_parent) factor = 1;
    }

    ul node, parent;
    if(factor == -1) {
      node = old_node;
      parent = *old_parent;
      old_parent++;
    }
    else {
      node = new_node;
      parent = *new_parent;
      new_parent++;
    }
    if(g.get_degOut(parent) > heap.huge) continue;
    heap.lazyIncrement(parent, factor*locality_parent(g, node, parent));

    for (auto sibling : g.neighOut_iter(parent)) {
      if(sibling == node) continue;
      heap.lazyIncrement(sibling, factor*locality_sibling(g, node, sibling));
    }
  }
}
