#ifndef ORDER_RAND
#define ORDER_RAND

//#include "tools.cpp"
#include "../utils/adjlist.cpp"


vector<ul> order_rand(Adjlist &g) {
	Debug("Order rand")
    vector<ul> rank;
    rank.reserve(g.n);
    for (ul i = 0; i < g.n; ++i)
        rank.push_back(i);
    
    srand ( unsigned ( time(0) ) );
    random_shuffle( rank.begin(), rank.end() );
    return rank;
}

#endif
