#ifndef ORDER_DEG_H
#define ORDER_DEG_H


//#include "tools.h"
#include "../utils/tools.h"
struct Keyvalue;
struct Edgelist;
struct Adjlist;

inline bool compare_nodedeg (Keyvalue &a, Keyvalue &b);


// --------------------------------------------------
// ----------------- From edgelist ------------------
// --------------------------------------------------
std::vector<ul> rank_from_deg(const std::vector<ul> &deg, const ul &n);
std::vector<ul> order_deg(Edgelist &g);
std::vector<ul> order_degOut(Edgelist &g);
std::vector<ul> order_degIn(Edgelist &g);

// --------------------------------------------------
// ----------------- From adjlist -------------------
// --------------------------------------------------
std::vector<ul> order_degOut(Adjlist &g);
std::vector<ul> order_degIn(Adjlist &g);

#endif
