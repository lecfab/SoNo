#include <iostream>
#include <iomanip>
#include <string>

#include <functional>
#include <vector>
#include <cmath>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand


#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/edgelist.h"

using namespace std;



int main(int argc, char** argv){
	TimeBegin()
  if(argc <= 2) {
    cout << "./bipartite EDGES OUTPUT -OPT" << endl;
    cout << "* EDGES is the initial edge list" << endl;
    cout << "* OUTPUT is the file to output the resulting edgelist" << endl;
    cout << "* OPT is a list of optional letters: r for random bipartition" << endl;
    cout << "returns a (sorted) edge list keeping only edges crossing between 2 subsets of nodes" << endl;
    return 1;
  }
  function<bool(string)> has_option = [&argc, &argv] (string o) -> bool {
    if(argc <= 3) return false;
    string options = string(argv[3]);
    return (options.find(o) != string::npos); // true if found
  };


  Info("Reading edgelist from file " << argv[1])
  ifstream edgefile(argv[1], ios::binary);
  Edgelist h(edgefile);
  Info("Number of nodes: " << h.n)
  Info("Number of edges: " << h.e)
  TimeStep("Read edges")

  Info("Creating bipartition")
  vector<ul> rank = order_identity(h.n);
	if(has_option("r")) {
		Info("Randomised bipartition")
		srand ( unsigned ( time(0) ) );
  	random_shuffle( rank.begin(), rank.end() );
	}


  Info("Output into file " << argv[2])
  ul e = 0;
  FILE *file = fopen(argv[2],"w");
  for(auto &edge : h.edges) {
    if(rank[edge.first] % 2 == rank[edge.second] % 2) continue; // edge is within one subset
    e ++;
    // if(rank[edge.first] % 2 == 0) // u is top
      fprintf(file, "%lu %lu\n", edge.first, edge.second);
    // else // u is bottom
    //   fprintf(file, "%lu %lu\n", edge.second, edge.first);
  }
  fclose(file);
  TimeStep("Output")

  Info("Remaining edges: "<< e)

  TimeTotal()
	return 0;
}
