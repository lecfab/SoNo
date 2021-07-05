#include <iostream>
#include <iomanip>
#include <string>

#include <functional>
#include <vector>
#include <cmath>
#include <algorithm>

#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/edgelist.h"

using namespace std;



int main(int argc, char** argv){
	TimeBegin()
  if(argc <= 3) {
    cout << "./rankedges EDGES RANK OUTPUT -OPT" << endl;
    cout << "* EDGES is the initial edge list" << endl;
    cout << "* RANK is the list of new indices for each node" << endl;
    cout << "* OUTPUT is the file to output the resulting edgelist" << endl;
    cout << "* OPT is a list of optional letters: r to reverse, u for undirected graphs" << endl;
    cout << "returns a (sorted) edge list where nodes have new indices defined by RANK" << endl;
    return 1;
  }
  function<bool(string)> has_option = [&argc, &argv] (string o) -> bool {
    if(argc <= 4) return false;
    string options = string(argv[4]);
    return (options.find(o) != string::npos); // true if found
  };


  Info("Reading edgelist from file " << argv[1])
  ifstream edgefile(argv[1], ios::binary);
  Edgelist h(edgefile);
  Info("Number of nodes: " << h.n)
  Info("Number of edges: " << h.e)
  TimeStep("Read edges")

  vector<ul> rank;
  if(string(argv[2]) == "original")
    rank = order_identity(h.n);
  else {
    Info("Reading rank from file " << argv[2])
    ifstream rankfile(argv[2]);
    rank = read_order(rankfile);
  }

  if(has_option("r")) { // reverse order
    Info("Reversing rank")
    for(ul u=0; u<h.n; ++u)
      rank[u] = h.n - 1 - rank[u];
  }
  TimeStep("Read rank")

  Info("Applying rank on edges")
  h.apply_rank(rank);
  TimeStep("Apply")

  if(has_option("u")) { // for an undirected graph, ensure u<v for each edge {u,v}
    Info("Creating DAG")
    h.to_dag();
    TimeStep("DAG")
  }

  Info("Sorting edges according to new indices")
  h.sort_edges();
  TimeStep("Sort")

  Info("Output into file " << argv[3])
  h.print_c(argv[3]);
  h.print_c_binary("binary_edgelist.bin");
  TimeStep("Output")

  TimeTotal()
	return 0;
}
