#include <vector>

#include "heap.h"
#include "tools.h"

using namespace std;

Bheap::Bheap(ul n_max) : n_max(n_max) {
	ul i;
	n = 0;
	pt.reserve(n_max);
	kv.reserve(n_max);
}

void Bheap::binary_swap(ul i, ul j) {
	std::swap(pt[ kv[i].key ], pt[ kv[j].key ]);
	std::swap(kv[i], kv[j]);
}

void Bheap::bubble_up(ul child) {
	while (child > 0) {
		ul parent = (child-1) / 2;
		if (kv[parent].val <= kv[child].val) break;
		binary_swap(child, parent);
		child = parent;
	}
}

void Bheap::bubble_down() {
	ul parent=0, left=1, right=2, child;
	while(left < n) {
		if(right < n and kv[right].val < kv[left].val)
			 child = right;
		else child = left;

		if(kv[parent].val <= kv[child].val) break;

		binary_swap(parent, child);
		parent = child;
		left = 2*parent + 1;
		right = left + 1;
	}
}

void Bheap::insert(Keyvalue new_kv) {
	kv.push_back(new_kv);
	pt[new_kv.key] = n;
	bubble_up(n);
  n++;
}

ul Bheap::key_to_val(ul key) {
	return kv[pt[key]].val;
}

void Bheap::update_decrement(ul key){
	ul i = pt[key];
	if (i > n_max) return;
	kv[i].val --;
	bubble_up(i);
}

void Bheap::update(ul key, ul new_val){
	ul i = pt[key];
	if (i > n_max) return;
	kv[i].val = new_val;
	bubble_up(i);
}

Keyvalue Bheap::popmin() {
	Keyvalue min = kv[0];
	pt[min.key] = n_max+1;
	kv[0] = kv[--n];
	pt[kv[0].key] = 0;
	bubble_down();
	return min;
}
