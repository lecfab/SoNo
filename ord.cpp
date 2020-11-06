#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>

#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

#include "utils/CLI11.h" // options parser
#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/adjlist.h"
#include "utils/edgelist.h"
#include "order/order_rand.h"
#include "order/order_deg.h"
#include "order/order_core.h"
#include "order/order_rcm.h"
#include "order/order_gorder.h"
#include "algo/algo_tarjan.h"
#include "algo/algo_kcore.h"

using namespace std;


int main(int argc, char** argv) {
	TimeBegin()

  CLI::App app{"Read a list of edges and permute its nodes according to various orders. See README.md for more information."};

  string filename, order_name="original", algo_name="no", output_file="ord.auto.txt";
  bool directed;
  app.add_option("file", filename, "Text file: list of `a b` edges with nodes IDs ranging from 0 to N-1")->required();
  app.add_option("order", order_name, "Order used to relabel the nodes")->required()
    ->check(CLI::IsMember({"original", "rand", "deg+", "deg-", "core", "core+", "core-", "icore+", "icore-", "rcm", "gorder"}, CLI::ignore_case));
  app.add_flag("-d,!-u,--directed,!--undirected", directed, "Specify if the graph is directed or undirected; multiple edges are not accepted");
  app.add_option("-o,--output", output_file, "File in which to output the order")->capture_default_str();

  CLI11_PARSE(app, argc, argv);


	ifstream file(filename);
  //ofstream output(output_file);
  ul n = 0;
  vector<ul> rank;


	// --------------------------------------------------
	// --- Sort after reading without data-structure ----
	// --------------------------------------------------

	if(order_name == "original" or order_name == "rand") {
		n = get_number_vertices(file);
    rank.reserve(n);
		for (ul u = 0; u < n; ++u) rank.push_back(u);
		TimeStep("Read")

		if(order_name == "rand") {
			Debug("Order rand from filestream")
			srand ( unsigned ( time(0) ) );
    	random_shuffle( rank.begin(), rank.end() );
		}
	}


	// --------------------------------------------------
	// -------- Store the graph in an edgelist ----------
	// --------------------------------------------------

	else {
		Info("Reading edgelist from file " << filename)
		Edgelist h(file); //*h = c_readedgelist(argv[1]);
		n = h.n;
    rank.reserve(n);

		Info("Number of nodes: " << h.n)
		Info("Number of edges: " << h.e)
		TimeStep("Read")

		if(order_name == "deg") rank = order_deg(h);
    else if(order_name == "deg+") rank = order_degOut(h);
		else if(order_name == "deg-") rank = order_degIn(h);
		else if(order_name == "rcm") rank = order_rcm(h);
    else if(order_name == "gorder") {
      rank = complete_gorder(h);
      // h.apply_rank(rank);
      // h.sort_edges();
    }


		// --------------------------------------------------
		// -- Convert edgelist file into Adjlist structure --
		// --------------------------------------------------

    else if(order_name == "core-" or order_name == "core+" or order_name == "icore-" or order_name == "icore+") {
      Badjlist g(h);
      if(order_name == "core-") rank = algo_kcoreIn(g).rank;
      else if(order_name == "core+") rank = algo_kcoreOut(g).rank;
      else if(order_name == "icore-") rank = algo_icoreIn(g).rank;
      else if(order_name == "icore+") rank = algo_icoreOut(g).rank;
    }
		else {
			Info("Converting to adjacency list");
      Adjlist* g;
      if(directed) g = new Dadjlist(h);
      else g = new Uadjlist(h);
			TimeStep("Adjlist")

      if(order_name == "core") rank = algo_kcore(*g).rank;
			else if(order_name == "tarjan") rank = algo_tarjan(*g);
			else { Alert("Unknown order `" << order_name <<"`"); return 1; }
      delete g;
    }
	}

	TimeStep("Order")

	// --------------------------------------------------
	// -------- Output the rank for further use ---------
	// --------------------------------------------------

	Info("Output into file " << output_file)
	// ofstream output_file(argv[4]); printorder(rank, g.n, output_file, g);
	c_printorder(rank, n, output_file.c_str());
	TimeStep("Output")
  TimeTotal()

	return 0;
}
