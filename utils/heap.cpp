#ifndef HEAP
#define HEAP

#include <iostream>
#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files
#include <ctime>   //to estimate the runing time
#include <string>
#include <cstring>

#include <vector>
#include <tuple>
#include <list>
#include <algorithm>

#include "tools.cpp"

using namespace std;

struct Bheap {
	ul n_max;
	ul n;
	vector<ul> pt;
	vector<Keyvalue> kv;

	Bheap(ul n_max) : n_max(n_max) {
		ul i;
		n = 0;
		pt.resize(n_max);
		//kv.resize(n_max);
	}

	void binary_swap(ul i, ul j) {
		std::swap(pt[ kv[i].key ], pt[ kv[j].key ]);
		std::swap(kv[i], kv[j]);
	}

	void bubble_up(ul child) {
		while (child > 0) {
			ul parent = (child-1) / 2;
			if (kv[parent].val <= kv[child].val) break;
			binary_swap(child, parent);
			child = parent;
		}
	}

	void bubble_down() {
		ul parent=0, left=1, right=2, child;
		while(left < n) {
			if(right < n and kv[right].val < kv[left].val)
				 child = right;
			else child = left;

			if(kv[parent].val < kv[child].val) break;

			binary_swap(parent, child);
			parent = child;
			left = 2*parent + 1;
			right = left + 1;
		}
	}

	void insert(Keyvalue new_kv) {
		kv.push_back(new_kv);
		pt[new_kv.key] = n ++;
		bubble_up(n-1);
	}

	ul key_to_val(ul key) {
		return kv[pt[key]].val;
	}

	void update(ul key, ul new_val){
		ul i = pt[key];
		if (i > n_max) return;
		kv[i].val = new_val;
		bubble_up(i);
	}

	Keyvalue popmin() {
		Keyvalue min = kv[0];
		pt[min.key] = n_max+1;
		kv[0] = kv[--n];
		pt[kv[0].key] = 0;
		bubble_down();
		return min;
	}
};


#endif
