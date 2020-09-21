#include <iostream>
#include <iomanip> 
#include <string>

#include <vector>
#include <cmath>

#include "utils/tools.cpp"
#include "utils/inout.cpp"
#include "utils/adjlist.cpp"
#include "order/order_rand.cpp"
#include "order/order_deg.cpp"
#include "order/order_core.cpp"

using namespace std;

int main(int argc, char** argv){
	TimeBegin()

	if(argc <= 3) { cout << "See README.txt" << endl; return 1; }
	string filename(argv[1]);
	ifstream file(filename);
	string order_name = string(argv[2]);
	ul n = 0;
	vector<ul> rank;


	// --------------------------------------------------
	// ------- Sort after reading without storing -------
	// --------------------------------------------------

	if(order_name == "no" or order_name == "rand") {
		n = get_number_vertices(file);
		for (ul u = 0; u < n; ++u) rank.push_back(u);
		TimeStep("Read")
		
		if(order_name == "rand") { Debug(n)
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
		edgelist *h = new edgelist; //*h = c_readedgelist(argv[1]);
		*h=readedgelist(file);
		n = h->n;

		Info("Number of nodes: " << h->n)
		Info("Number of edges: " << h->e)
		TimeStep("Read")

		if(order_name == "deg") rank = order_deg(*h);
		else if(order_name == "deg-") rank = order_degIn(*h);
		else if(order_name == "deg+") rank = order_degOut(*h);


		// --------------------------------------------------
		// -- Convert edgelist file into Adjlist structure --
		// --------------------------------------------------

		else {
			Info("Converting to adjacency list");
			Dadjlist g(*h);
			TimeStep("Adjlist")

			if(order_name == "core") rank = order_core(g);
			else { Alert("Unknown order `" << order_name <<"`"); return 1; }
		}
		delete h;
	}

	TimeStep("Order")

	// --------------------------------------------------
	// -------- Output the rank for further use ---------
	// --------------------------------------------------

	Info("Output into file " << argv[3])
	// ofstream output_file(argv[4]); printorder(rank, g.n, output_file, g);
	c_printorder(rank, n, argv[3]);
	TimeStep("Output")



	TimeTotal()
	return 0;
}
