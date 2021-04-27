#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>
#include <algorithm>

#include "utils/CLI11.h" // options parser
#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/edgelist.h"
#include "utils/adjlist.h"
#include "algo/algo_bfs.h"
#include "algo/algo_dfs.h"
// #include "order/order_minla.h"


using namespace std;


double energy_rank(const Adjlist &g, const vector<ul> &rank, double norm(double)) {
  double e = 0;
  for (ul u = 0; u < g.n; u++) {
    const double rank_u = rank[u];
    for(const ul &v : g.neigh_iter(u)) {
      // if(v > u) continue;
      e += norm(rank_u - ((double)rank[v]));
    }
  }
  return e;
}

double energy_2ranks(const Adjlist &g, const vector<ul> &r1, const vector<ul> &r2, double norm(double)) {
  double e = 0;
  for (ul u = 0; u < g.n; u++) {
    const double r1_u = r1[u],  r2_u = r2[u];
    for(const ul &v : g.neigh_iter(u)) {
      // if(v > u) continue;
      e += min(norm(r1_u - ((double)r1[v])), norm(r2_u - ((double)r2[v])));
    }
  }
  return e;
}


double egap_u(const Adjlist &g, const ul &u, const vector<ul> &rank, double norm(double)) {
  vector<double> neigh; neigh.reserve(g.get_degree(u));
  for(const ul &v : g.neigh_iter(u))
    neigh.push_back((double) rank[v]);
  sort(neigh.begin(), neigh.end());

  double e = 0;
  double previous = rank[u]; // note that first neighbour can be before u, in which case BV uses Z->N mapping (not represented here)
  for(auto &rank_v : neigh) {
    e += norm(rank_v - previous);
    previous = rank_v;
  }
  return e;
}
double egap_rank(const Adjlist &g, const vector<ul> &rank, double norm(double)) {
  double e = 0;
  for (ul u = 0; u < g.n; u++)
    e += egap_u(g, u, rank, norm);
  return e;
}

double egap_2ranks_full(const Adjlist &g, const vector<ul> &r1, const vector<ul> &r2, double norm(double)) {
  double emin = 0, emax = 0;
  for (ul u = 0; u < g.n; u++) {
    double e1 = egap_u(g, u, r1, norm);
    double e2 = egap_u(g, u, r2, norm);
    emin += min(e1, e2);
    emax += max(e1, e2);
  }
  Info("min / max : " << emin/emax)
  return emin;
}

inline double log_abs(double x) { return log2(abs(x)); }

int main(int argc, char** argv){
	TimeBegin()

  CLI::App app{"Test an algorithm with different parameters to tune them efficiently."};

  string filename, algo_name, output_file="parametrise.auto.txt";
  int range = 9, number = 100;
  // bool directed;
  app.add_option("file", filename, "Text file: list of `a b` edges with nodes IDs ranging from 0 to N-1")->required();
  // app.add_option("algo", algo_name, "Algorithm with parameters to tune")->required()
  //   ->check(CLI::IsMember({"minla", "minloga","gorder","local"}, CLI::ignore_case));
  // app.add_flag("-d,!-u,--directed,!--undirected", directed,
  //     "Specify if the graph is directed or undirected; multiple edges are not accepted");
  // app.add_option("-o,--output", output_file, "File in which to output the order")->capture_default_str();

  CLI11_PARSE(app, argc, argv);

  Info("Reading edgelist from file " << argv[1])
  ifstream file(filename);
  Edgelist h(file);
  Info("Number of nodes: " << h.n)
  Info("Number of edges: " << h.e)
  TimeStep("Read edges")

  Info("Converting to bidirected adjacency list");
  Uadjlist g(h);
  TimeStep("Adjlist")

  vector<ul> bfs = rank_from_order(algo_bfs(g));
  vector<ul> dfs = algo_dfs(g, g.n/2);

  Info("Minla")
  double minla_bfs = energy_rank(g, bfs, abs);
  double minla_dfs = energy_rank(g, dfs, abs);
  double minla_both = energy_2ranks(g, bfs, dfs, abs);
  Info("bfs " << minla_bfs <<", per edge "<<(minla_bfs / ((double)g.e)))
  Info("dfs " << minla_dfs <<", per edge "<<(minla_dfs / ((double)g.e)))
  Info("both " << minla_both <<", per edge "<<(minla_both / ((double)g.e)))
  TimeStep("Minla")

  Info("Minloga")
  double minloga_bfs = energy_rank(g, bfs, log_abs);
  double minloga_dfs = energy_rank(g, dfs, log_abs);
  double minloga_both = energy_2ranks(g, bfs, dfs, log_abs);
  Info("bfs " << minloga_bfs <<", per edge "<<(minloga_bfs / ((double)g.e)))
  Info("dfs " << minloga_dfs <<", per edge "<<(minloga_dfs / ((double)g.e)))
  Info("both " << minloga_both <<", per edge "<<(minloga_both / ((double)g.e)))
  TimeStep("Minloga")

  Info("Lingap")
  double mlgapa_bfs = egap_rank(g, bfs, abs);
  double mlgapa_dfs = egap_rank(g, dfs, abs);
  double mlgapa_both = egap_2ranks_full(g, bfs, dfs, abs);
  Info("bfs " << mlgapa_bfs <<", per edge "<<(mlgapa_bfs / ((double)g.e)))
  Info("dfs " << mlgapa_dfs <<", per edge "<<(mlgapa_dfs / ((double)g.e)))
  Info("both " << mlgapa_both <<", per edge "<<(mlgapa_both / ((double)g.e)))
  TimeStep("Mgap")

  Info("Loggap")
  double mloggapa_bfs = egap_rank(g, bfs, log_abs);
  double mloggapa_dfs = egap_rank(g, dfs, log_abs);
  double mloggapa_both = egap_2ranks_full(g, bfs, dfs, log_abs);
  Info("bfs " << mloggapa_bfs <<", per edge "<<(mloggapa_bfs / ((double)g.e)))
  Info("dfs " << mloggapa_dfs <<", per edge "<<(mloggapa_dfs / ((double)g.e)))
  Info("both " << mloggapa_both <<", per edge "<<(mloggapa_both / ((double)g.e)))
  TimeStep("Loggap")

  TimeTotal()
	return 0;
}
