#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>
#include <algorithm>

#include "utils/tools.h"

using namespace std;

// inline ul max3(ul a, ul b, ul c) { return (a >= b and a >= c) ? a : ((b >= c) ? b : c); /}

int main(int argc, char** argv){
	TimeBegin()
  if(argc <= 2) {
    cout << "./undirect INPUT OUTPUT" << endl;
    cout << "with INPUT the initial edgelist, OUTPUT the file to output the trimmed edgelist" << endl;
    cout << "returns a (sorted) list of non-redundant edges (if both `a b` and `b a` existed, only `a b` is kept)" << endl;
    return 1;
  }
  cout << "Read file " << argv[1] << endl;
  ifstream filein(argv[1]);

  vector<pair<ul,ul>> edges;
	edges.reserve(NLINKS);
	ul n=0;
	ul u, v;
	while(filein >> u >> v) {
    if(u > v) swap(u, v);
		edges.push_back(make_pair(u, v));
		n = max3(n, u, v);
    // cout << "read " << u << " " << v << endl;
	}
	ul e = edges.size();
	n++;

  TimeStep("Read")

  cout << "Sort " << e << " edges" << endl;

  sort(edges.begin(), edges.end());
  TimeStep("Sort")

  cout << "Write file " << argv[2] << endl;
  FILE *fileout = fopen(argv[2], "w");
  u = v = n + 2;
  ul e2 = 0;
	for (ul i=0; i < e; ++i) {
    if(edges[i].second == v and edges[i].first == u) continue;
    e2 ++;
    u = edges[i].first;
    v = edges[i].second;
    fprintf(fileout, "%lu %lu\n", u, v);
    // cout << "written " << u << " " << v << endl;
  }
	fclose(fileout);

  cout << "Output " << e2 << " edges" << endl;
  TimeStep("Out")

  TimeTotal()
	return 0;
}
