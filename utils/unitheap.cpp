// MIT License Copyright (c) 2016, Hao Wei.

#include "tools.h"
#include "unitheap.h"
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

void UnitHeap::print_status() {
  #ifdef DEBUG
  safety_check();
  Debug("----------------- Heap status -----------------")
  Debug("| Top: " << top)
  for (size_t i = 0; i < 22; i++) {
    Debug("| "<<i << " key:" << LinkedList[i].key << " up:" << update[i]
        << "\t\tprev: "<<LinkedList[i].prev << " next:" << LinkedList[i].next)
  }
  Debug("----------------- ---- ------ -----------------")
  for(int i=0; i <Header.size(); i++) {
    auto h = Header[i];
    Debug("| Level "<<i << ": "<<h.first<<" to "<<h.second)
  }
  Debug("----------------- ---- ------ -----------------")
  #endif
  return;
}

void UnitHeap::safety_check() {
  int problem = 0;

  int key_out_header = 0, first_not_first = 0, last_not_last = 0;
  int beginners = 0, enders = 0;
  for (size_t i = 0; i < LinkedList.size(); i++) {
    int key = LinkedList[i].key;
    if(update[i]>=infty) continue;
    ul first = Header[key].first, second = Header[key].second;
    // keys are well in their header value
    if(key != LinkedList[first].key or key != LinkedList[second].key)
    {
      Debug("out: "<<key<<" "<<LinkedList[first].key<<" "<<LinkedList[second].key)
      key_out_header += 1;
    }
    // header first is lower than prev
    if(first == i and LinkedList[i].prev != none)
      if(LinkedList[LinkedList[i].prev].key <= key)
        first_not_first ++;
    // header last is higher than next
    if(second == i and LinkedList[i].next != none)
      if(LinkedList[LinkedList[i].next].key >= key)
        last_not_last ++;
    // only one beginner and ender
    if(LinkedList[i].prev == none) beginners += 1;
    if(LinkedList[i].next == none) enders += 1;
  }
  if(key_out_header and ++problem) Alert("safety_check: key_out_header " << key_out_header)
  if(first_not_first and ++problem) Alert("safety_check: first_not_first " << first_not_first)
  if(last_not_last and ++problem) Alert("safety_check: last_not_last " << last_not_last)
  if(beginners != 1 and ++problem) Alert("safety_check: beginners " << beginners)
  if(enders != 1 and ++problem) Alert("safety_check: enders " << enders)

  // top is not none
  if(top == none and ++problem) Alert("safety_check: top_none")
  // top has no prev
  else if(LinkedList[top].prev != none and ++problem) Alert("safety_check: top_prev "<<LinkedList[top].prev)

  // heapsize and LinkedList size are the same
  ul u = top; int chain_size = 0;
  while(u!=none && ++chain_size && chain_size < heapsize*2)
    u = LinkedList[u].next;
  if(chain_size != heapsize and ++problem) Alert("safety_check: chain_size "<<chain_size<<" not "<<heapsize)

  if(problem) exit(1);
  Info("safety_check done")
}

UnitHeap::UnitHeap(ul size) {
  none = size + 2;
  Debug("none is "<< none << ", infinity is "<<infty)
  huge = sqrt(size); // should be in ReConstruct with actual heapsize
  LinkedList.resize(size, {.key=infty,.prev=none,.next=none});
  update.resize(size, infty);
}
void UnitHeap::InsertElement(const ul index, const int key) {
  LinkedList[index].key = key;
  update[index] = -key;
  heapsize++;
}

void UnitHeap::ReConstruct() {
  vector<ul> g(heapsize);
  for (ul i = 0; i < heapsize; i++)
    g[i] = i;

  sort(g.begin(), g.end(), [&](const ul a, const ul b) -> bool {
    return (LinkedList[a].key > LinkedList[b].key);
  }); // degree DESC
  // sorting update should be simpler as it's a straightforward array
  // sort(g.begin(), g.end(), [&](const int a, const int b) -> bool {
  //   return (update[a] < update[b]);
  // });

  top = g[0];
  int current_key = LinkedList[top].key; // max degree
  Header.resize(10*current_key + 1, {.first=none,.second=none});
  Header[current_key].first = top;
  for (size_t i = 0; i < g.size(); i++) {
    ul v = g[i];
    LinkedList[v].prev = (i>0) ? g[i-1] : none;
    LinkedList[v].next = (i<g.size()-1) ? g[i+1] : none;

    int key = LinkedList[g[i]].key;
    if (key != current_key) {
      Header[current_key].second = g[i - 1];
      Header[key].first = g[i];
      current_key = key;
    }
  }

  Header[current_key].second = g.back();
  print_status();
}


void UnitHeap::DeleteElement(const ul index) {
  Debug("Delete "<< index)
  update[index] = infty;
  ul prev = LinkedList[index].prev;
  ul next = LinkedList[index].next;
  int key = LinkedList[index].key;

  if (prev != none) LinkedList[prev].next = next;
  if (next != none) LinkedList[next].prev = prev;

  erase_key_element(index, next, prev);

  if (top == index)
    top = next;

  LinkedList[index].prev = LinkedList[index].next = none;
  // heapsize --;
  if(--heapsize % (LinkedList.size()/20) == 1) safety_check();
}

void UnitHeap::DecreaseTop() {
  // If the "top" has to be updated, try and update half what you should
  // and see if someone else comes on top
  // Recursion will take care of further updates

  const ul next = LinkedList[top].next;
  if(next == none) {
    Debug("next == none: ended heap with "<< top)
    return;
  }

  const int key = LinkedList[top].key;
  const int leftover = update[top] / 2;
  const int new_key = key + update[top] - leftover; //*//
  Debug("Decrease "<<top<<" = "<<key<<" "<<update[top])

  if (new_key >= LinkedList[next].key) return;
  update[top] = leftover; //*//


  ul level_tail = Header[key].second;
  ul next_level = LinkedList[level_tail].next;
  int loops = 0;
  while (next_level != none && LinkedList[next_level].key >= new_key) {
    level_tail = Header[LinkedList[next_level].key].second;
    next_level = LinkedList[level_tail].next;
    if(++loops%huge==0) {
      Debug("Looped " << loops << " times on " << top << " from key "<<key<<" to "<<new_key<<" between "<<level_tail<<" and "<<next_level<<" having "<<LinkedList[next_level].key)
      safety_check();
    }
  }
  if(loops >= huge) Debug("loophole escaped")
  Debug(top << " from key "<<key<<" to "<<new_key<<" between "<<level_tail<<" and "<<next_level)

  LinkedList[next].prev = none;
  LinkedList[top].prev = level_tail;
  LinkedList[top].next = next_level;
  LinkedList[level_tail].next = top;
  if (next_level != none) LinkedList[next_level].prev = top;

  if (top != Header[key].first) Alert("Header not first")
  erase_key_element(top, next, none);

  LinkedList[top].key = new_key;
  Header[new_key].second = top;
  if (Header[new_key].first == none)
    Header[new_key].first = top;
  // Header[new_key].first = top;
  // if(Header[new_key].second == none)
  //   Header[new_key].second = top;

  top = next;
}

void UnitHeap::erase_key_element(const ul index, const ul next, const ul prev) {
  int key = LinkedList[index].key;
  if (Header[key].first == Header[key].second) // == index
    Header[key].first = Header[key].second = none;
  else if (index == Header[key].first)
    Header[key].first = next;
  else if (index == Header[key].second)
    Header[key].second = prev;
}

ul UnitHeap::ExtractMax() {
  Debug("Extract from candidates "<<heapsize)
  ul tmptop;
  int loops=0;
  do {
    tmptop = top;
    if (update[top] < 0) {
      DecreaseTop();
    }
    if(++loops%100000==0) Debug(loops <<" exctract loophole "<<top<<" "<<tmptop)
  } while (top != tmptop);

  Debug("extracted" << tmptop)
  DeleteElement(tmptop);
  return tmptop;
}





void UnitHeap::IncrementKey(const ul index) {
  const ul level_head = Header[LinkedList[index].key].first;
  const ul prev = LinkedList[index].prev;
  const ul next = LinkedList[index].next;
  Debug("Increment "<<index<<" = "<<LinkedList[index].key<<"+"<<update[index]<<" between "<<prev<<" and "<<next)

  if (level_head != index) { // if index is head of level, keep LinkedList
    LinkedList[prev].next = next;
    if (next != none)
      LinkedList[next].prev = prev;

    ul prev_level = LinkedList[level_head].prev;
    LinkedList[index].prev = prev_level;
    LinkedList[index].next = level_head;
    LinkedList[level_head].prev = index;
    if (prev_level != none)
      LinkedList[prev_level].next = index;
  }

  erase_key_element(index, next, prev);

  int key = ++LinkedList[index].key;
  Header[key].second = index;
  if (Header[key].first == none) {
    Header[key].first = index;
    if (key > LinkedList[top].key)
      top = index; // was not in the if initially
  }

  if (key + 14 >= (int)Header.size())
    Header.resize(Header.size() * 1.5, {.first=none,.second=none});
}

void UnitHeap::lazyIncrement(const ul index, const int up) {
  if (update[index] == infty) return;
  if (update[index] == 0 and up > 0)
    IncrementKey(index); // unlikely
  else
    update[index] += up; // up can be negative
}

// void UnitHeap::IncrementKey(const ul index) {
//   int key = LinkedList[index].key;
//   const ul head = Header[key].first;
//   const ul prev = LinkedList[index].prev;
//   const ul next = LinkedList[index].next;
//
//   if (head != index) {
//     LinkedList[prev].next = next;
//     if (next != none)
//       LinkedList[next].prev = prev;
//
//     ul headprev = LinkedList[head].prev;
//     LinkedList[index].prev = headprev;
//     LinkedList[index].next = head;
//     LinkedList[head].prev = index;
//     if (headprev != none)
//       LinkedList[headprev].next = index;
//   }
//
//   LinkedList[index].key++;
//   if (Header[key].first == Header[key].second)
//     Header[key].first = Header[key].second = none;
//   else if (Header[key].first == index)
//     Header[key].first = next;
//   else if (Header[key].second == index)
//     Header[key].second = prev;
//
//   key++;
//   Header[key].second = index;
//   if (Header[key].first == none)
//     Header[key].first = index;
//
//   if (LinkedList[top].key < key) {
//     top = index;
//   }
//
//   if (key + 4 >= (int)Header.size()) {
//     Header.resize(Header.size() * 1.5);
//   }
// }


// UnitHeap::UnitHeap(ul size) {
//   none = size + 2;
//   heapsize = 0; // size;
//   huge = sqrt(size);
//   if (size > 0) {
//     Header.clear();
//     Header.resize(size >> 4); // >> bitwise operation divides by 2⁴
//     // Header.resize(max((size >> 4) > INITIALVALUE + 4));
//     LinkedList = new ListElement[size];
//     update = new int[size];
//
//     for (size_t i = 0; i < size; i++) {
//       LinkedList[i].prev = i - 1;
//       LinkedList[i].next = i + 1;
//       LinkedList[i].key = INITIALVALUE;
//       update[i] = -INITIALVALUE;
//     }
//     LinkedList[size - 1].next = none;
//
//     Header[INITIALVALUE].first = 0;
//     Header[INITIALVALUE].second = size - 1;
//     top = 0;
//   }
// }

// void UnitHeap::DecrementKey(const ul index) { update[index]--; }

// void UnitHeap::DecreaseTop() {
//   // If the "top" has to be updated, try and update half what you should
//   // and see if someone else comes on top
//   // Recursion will take care of further updates
//   const ul tmptop = top;
//   const ul next = LinkedList[tmptop].next;
//   const int key = LinkedList[tmptop].key;
//   const int newkey = key + update[tmptop] - (update[tmptop] / 2); /**/
//
//   if (newkey >= LinkedList[next].key)
//     return;
//   LinkedList[next].prev = none;
//
//   ul psecond = Header[key].second;
//   ul tailnext = LinkedList[psecond].next;
//   while (tailnext != none && newkey < LinkedList[tailnext].key) {
//     psecond = Header[LinkedList[tailnext].key].second;
//     tailnext = LinkedList[psecond].next;
//   }
//
//   LinkedList[top].prev = psecond;
//   LinkedList[top].next = tailnext;
//   LinkedList[psecond].next = tmptop;
//   if (tailnext != none)
//     LinkedList[tailnext].prev = tmptop;
//   top = next;
//
//   if (Header[key].first == Header[key].second)
//     Header[key].first = Header[key].second = none;
//   else
//     Header[key].first = next;
//
//   LinkedList[tmptop].key = newkey;
//   update[tmptop] /= 2; /**/
//   Header[newkey].second = tmptop;
//   if (Header[newkey].first == none)
//     Header[newkey].first = tmptop;
// }

// void UnitHeap::DeleteElement(const ul index) {
//   ul prev = LinkedList[index].prev;
//   ul next = LinkedList[index].next;
//   int key = LinkedList[index].key;
//
//   if (prev != none)
//     LinkedList[prev].next = next;
//   if (next != none)
//     LinkedList[next].prev = prev;
//
//   if (Header[key].first == Header[key].second) {
//     Header[key].first = Header[key].second = none;
//   } else if (Header[key].first == index) {
//     Header[key].first = next;
//   } else if (Header[key].second == index) {
//     Header[key].second = prev;
//   }
//
//   if (top == index) {
//     top = LinkedList[top].next;
//   }
//   LinkedList[index].prev = LinkedList[index].next = none;
//   heapsize--;
// }

// void UnitHeap::ReConstruct() {
//   vector<ul> tmp(heapsize);
//   for (size_t i = 0; i < heapsize; i++)
//     tmp[i] = i;
//
//   sort(tmp.begin(), tmp.end(), [&](const int a, const int b) -> bool {
//     return (LinkedList[a].key > LinkedList[b].key);
//   });
//
//   int key = LinkedList[tmp[0]].key; // max degree
//   LinkedList[tmp[0]].prev = none;
//   LinkedList[tmp[0]].next = tmp[1];
//   LinkedList[tmp.back()].prev = tmp[tmp.size() - 2];
//   LinkedList[tmp.back()].next = none;
//   Header = vector<HeadEnd>(max(key + 1, (int)Header.size()));
//   Header[key].first = tmp[0];
//   for (size_t i = 1; i < tmp.size() - 1; i++) {
//     ul v = tmp[i];
//     LinkedList[v].prev = tmp[i - 1];
//     LinkedList[v].next = tmp[i + 1];
//
//     int tmpkey = LinkedList[tmp[i]].key;
//     if (tmpkey != key) {
//       Header[key].second = tmp[i - 1];
//       Header[tmpkey].first = tmp[i];
//       key = tmpkey;
//     }
//   }
//
//   int lastkey = LinkedList[tmp.back()].key;
//   if (key == lastkey) // lowest degree
//     Header[key].second = tmp.back();
//   else {
//     Header[key].second = tmp[tmp.size() - 2];
//     Header[lastkey].first = Header[lastkey].second = tmp.back();
//   }
//   top = tmp[0];
// }

// UnitHeap::~UnitHeap() {
//   delete[] LinkedList;
//   delete[] update;
// }
