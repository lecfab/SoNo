// MIT License Copyright (c) 2016, Hao Wei.

#ifndef _UNITHEAP_H
#define _UNITHEAP_H

#include "tools.h"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <vector>

const int INITIALVALUE = 0;

class ListElement {
public:
  int key;
  ul prev;
  ul next;
};

class HeadEnd {
public:
  ul first;
  ul second;
  // HeadEnd() { first = second = -1; }
};

class UnitHeap {
public:
  std::vector<int> update;
  std::vector<ListElement> LinkedList; // key=degree, prev=id of previous node
                                       // in degree DESC ordering, next=...
  std::vector<HeadEnd> Header; // first=id of first node with this degree,
                               // second=id of last node...
  size_t heapsize = 0;         // updated at each instant
  ul top;                      // id of node with highest degree
  ul huge;                     // ignore huge nodes (deg > sqrt of graph size)
  ul none;                     // integer that cannot be a node index
  const int infty = INT_MAX/2;

  UnitHeap(ul size);
  // ~UnitHeap();
  void InsertElement(const ul index, const int key);
  void DeleteElement(const ul index);
  ul ExtractMax();
  void IncrementKey(const ul index);
  void lazyIncrement(const ul index, const int up);
  void DecrementKey(const ul index);
  void DecreaseTop();
  void ReConstruct();
  void erase_key_element(const ul index, const ul next, const ul prev);
  void print_status();
  void safety_check();
};

#endif
