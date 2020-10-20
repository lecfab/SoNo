#ifndef ADJLIST_H
#define ADJLIST_H

/* Structures for compact adjacency lists, built from a list of edges */
// #include <vector>
// #include <tuple>
// #include <list>
// #include <algorithm>

#include "tools.h"
struct Edgelist;

// using namespace std;

struct NeighIter {
  const ul *begin_value;
  const ul *end_value;
  NeighIter(const ul *begin_value, const ul *end_value) : begin_value(begin_value), end_value(end_value) {}
  const ul* begin() const { return begin_value; }
  const ul* end() const { return end_value; }
};

struct Adjlist {
public:
	int edge_factor;
	ul n;//number of nodes
	ul e;//number of edges
	std::vector<ul> cd;//cumulative degree cd[0]=0, length=n+1
	std::vector<ul> adj;//concatenated lists of neighbors of all nodes, length e

	Adjlist(Edgelist &g, int edge_factor=1);

	// iterate over neighbours: for (auto v=neigh_beg(u); v<neigh_end(u); ++v)  cout << *v << endl;
  // or with iterator: for (auto &v : g.neigh_iter(u)) cout << v << endl;
	ul* neigh_beg(const ul &u);
	ul* neigh_end(const ul &u);
	const ul* neigh_beg(const ul &u) const;
	const ul* neigh_end(const ul &u) const;
  NeighIter neigh_iter(const ul &u);
  const NeighIter neigh_iter(const ul &u) const;

	ul get_degree(const ul &u) const;

	void print_some(int a);

	void reorder(const std::vector<ul> &rank);

	Edgelist to_edgelist(bool directed=true, auto ranker=[](ul u) { return u; });
	Edgelist to_edgelist_ranked(bool directed, const std::vector<ul> &rank);


protected: // Private building operations
	void build_from_edgelist(Edgelist &g, bool sorted=false);

    // pure virtual functions need to be implemented by inherited classes https://stackoverflow.com/a/1306837
	virtual std::vector<ul> compute_degrees(Edgelist &g) = 0;
	virtual void build_adj_array(Edgelist &g, std::vector<ul> &deg) = 0;
	virtual bool build_edge_condition(const ul &u, const ul &v) = 0;

	void compute_cumulated_degrees(std::vector<ul> &deg);
	void sort_neighbours();
};

struct Uadjlist : Adjlist {
	Uadjlist(Edgelist &g);
	std::vector<ul> compute_degrees(Edgelist &g);
	void build_adj_array(Edgelist &g, std::vector<ul> &deg);
	inline bool build_edge_condition(const ul &u, const ul &v) { return u < v; }
};

struct Dadjlist : Adjlist {
	Dadjlist(Edgelist &g);
	std::vector<ul> compute_degrees(Edgelist &g);
	void build_adj_array(Edgelist &g, std::vector<ul> &deg);
	inline bool build_edge_condition(const ul &u, const ul &v) { return true; }
};

#endif
