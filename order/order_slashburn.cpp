/*
https://datalab.snu.ac.kr/~ukang/

Define parameter k:
	k=1 is best but slower to precompute
	k=n/200 is recommanded

Loop:
	Remove k-hubest
	Order hubs first, giant connected component then, spokes finally

Removing:
	Take highest degree node
	Recompute degrees
	etc

Order of spokes:
	Instead of taking their component size, take the size of their former hub

*/


Input: Edge set E of a graph G = (V, E),
a constant k (default = 1).
Output: Array Γ containing the ordering V → [n].
1: Remove k-hubset from G to make the new graph G 0 . Add
the removed k-hubset to the front of Γ.
2: Find connected components in G 0 . Add nodes in
non-giant connected components to the back of Γ, in the
decreasing order of sizes of connected components they
belong to.
3: Set G to be the giant connected component (GCC) of G 0 .
Go to step 1 and continue, until the number of nodes in
the GCC is smaller than k

vector<ul> slashburn(const Dadjlist &g, ul k) {
	g = copy(g);
	vector<ul> rank;
	while(gcc.size() >= k) {
		for(ul i=0; i<k; ++i) {
			ul hub = g.find_hub();
			rank.push_back(hub);
			g.remove_node(hub);
		}

		g2 = GCC(g);
		slashburn(g2);
		rank.push_back(what remains);
	}
}

vector<ul> GCC(const Dadjlist &g, vector<ul> hubs)
{
	ul l=1;
	for (ul i = 0; i < k; ++i)
	{
		for(ul *u=g.neigh_beg(hubs[i]); u<g.neigh_end(hubs[i]); ++u) {
			if(!visited[*u]) {
				For every visited node ū, L(ū) = l
				Start Blocked-BFS by hubs at u with label l
				l++;
			}
		}
		T_L(u) = i;
	}
}

