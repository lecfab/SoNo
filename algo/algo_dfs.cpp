#ifndef ALGO_DFS
#define ALGO_DFS

#include <list>

#include "../utils/tools.cpp"
#include "../utils/adjlist.cpp"

using namespace std;


// procedure DFS_iterative(G, v) is
//     let S be a stack
//     S.push(iterator of G.adjacentEdges(v))
//     while S is not empty do
//         if S.peek().hasNext() then
//             w = S.peek().next()
//             if w is not labeled as discovered then
//                 label w as discovered
//                 S.push(iterator of G.adjacentEdges(w))
//         else
//             S.pop() 

struct Nodeneigh {
	const ul node;
	const ul* neigh;
	Nodeneigh(const Adjlist &g, ul &u) : node(u), neigh(g.neigh_beg(u)) {}
};
// time n+m, size n
vector<ul> algo_dfs(const Adjlist &g) {
	Debug("Algo dfs")
	vector<bool> visited(g.n, false);
	vector<Nodeneigh> stack; stack.reserve(g.n);
	vector<ul> rank; rank.reserve(g.n);

	ul r=0;

	for (ul u = 0; u < g.n; ++u) {
		if(visited[u]) continue;
	
		rank[u] = r++;
		visited[u] = true;
		stack.push_back(Nodeneigh(g, u));

		while(!stack.empty()) {
			Nodeneigh &deepest = stack.back();
			if(deepest.neigh >= g.neigh_end(deepest.node)) stack.pop_back();
			else {
				ul v = *deepest.neigh;
				deepest.neigh ++;
				if(visited[v]) continue;
				rank[v] = r++;
				visited[v] = true;
				stack.push_back(Nodeneigh(g, v));
			}
		}
	}

	for(ul u=0; u<g.n; u++) {
        cout << u << "\t" << rank[u] << endl;
    }

	return rank;
}


// time m+n, size m
// ul source_dfs(const Dadjlist &g, const ul u)
// {
// 	vector<bool> visited(g.n, false);
// 	vector<ul> toVisit; toVisit.reserve(g.n);
// 	toVisit.push_back(u);

// 	ul nb = 0;
// 	while(!toVisit.empty()) {
// 		ul u = toVisit.back();
// 		toVisit.pop_back();
// 		if(visited[u]) continue;
// 		visited[u] = true;
// 		nb ++;

// 		for (ul i = g.cd[u]; i < g.cd[u+1]; ++i) {
// 			ul v = g.adj[i];
// 			if(!visited[v])
// 				toVisit.push_back(v);
// 		}
// 		if(nb%10000==0) cout << nb << " " << toVisit.size() << endl;
// 	}

// 	return nb;
// }

#endif
