#ifndef HEAP_H
#define HEAP_H

#include <vector>

#include "tools.h"

struct Keyvalue;

// using namespace std;

struct Bheap {
  ul n_max;
  ul n;
  std::vector<ul> pt;
  std::vector<Keyvalue> kv;

  Bheap(ul n_max);

  void insert(Keyvalue new_kv);
  void update(ul key, ul new_val);
  void update_decrement(ul key);

  Keyvalue popmin();

  void binary_swap(ul i, ul j);
  void bubble_up(ul child);
  void bubble_down();
  ul key_to_val(ul key);
};

#endif
