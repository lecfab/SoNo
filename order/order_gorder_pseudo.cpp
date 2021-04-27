int main(int argc, char* argv[]){
  int w;
  string filename;

  Graph g;
  g.readGraph(filename);
  g.Transform();

  vector<int> order;
  g.GorderGreedy(order, w);

  g.PrintReOrderedGraph(order);
  return 0;
}

Graph {
  vsize, edgenum, name;
  vector<Vertex> graph;
  vector<int> outedge;
  vector<int> inedge;
}

Vertex {
  int outstart, outdegree, instart, indegree;
}


inline locality_parent(u, v) { return 1; }
inline locality_child(u, v) { return 1; }
inline locality_sibling(u, v) { return 1; }

void heap_increment(w, locality, add=true) {
  if(add) {
    if(heap.update[w] >= 0) heap.IncrementKey(w); // unlikely
    else heap.update[w] += locality;
  }
  else heap.update[w] -= locality;
}

void move_window(v, add=true) {
  if(add) heap.update[u] = infinity;

  for each in neighbour u of v {
    if(u huge) continue;
    heap_increment(u, locality_parent, add);

    for each out neighbour w of u
      heap_increment(w, locality_sibling, add);
       // note: while removing v, if u->v then w will gain this again as sibling of v
  }

  if (v huge) return;
  for each out neighbour w of v
    heap_increment(w, locality_child, add);
}


Graph::GorderGreedy(vector<int> &retorder, int window) {
  UnitHeap heap of size visze;
  vector<int> order; .reserve(vsize);

  const int huge = sqrt(visze); // max out degree considered

  vector<int> zero; .reserve(10000);
  zero = strongly isolated nodes (removed)

  heap.LinkedList[i].key = indegree[i];
  heap.update[i] = - indegree[i];
  heap.ReConstruct(); // heap indegree DESC sort

  find node u of max degree
  order.push_back(u);
  heap.DeleteElement(u);
  move_window(u);

  int count=0;
  while (count < vsize - 1 - zero.size()) {
    v = heap.ExtractMax();
    order.push_back(v);
    move_window(v);

    if(count-window >= 0) {
      u = order[count - window];
      move_window(u, false);
    }
  }
  order.push_back(all the zero array) // order.insert(order.end() - 1, zero.begin(), zero.end());

  retorder[order[i]] = i;
}




Graph::Transform() {
  vector<int> order = RCMOrder(); // stupid BFS with low degree neighbour first
  vector<int> inedge empty;
  edges = edgelist with the new order
  recompute indegree and outdegree
  recompute outstart and instart
  sort edges u ASC, v ASC ;
  fill outedge array;
  fill inedge array;
}

// Graph::RCMOrder(vector<int>& retorder) {
//   queue<int> que;
//   vector<bool> BFSflag of size n;
//   vector<bool> QueFlag of size n;
//   vector<int> tmp;
//   vector<int> degreeVertex = nodes sorted by total degree;
//
//   int now;
//   vector<int> order;
//
//   for each index k {
//     i = degreeVertex[k];
//     if(BFSflag[i]) continue;
//     que.push(i);
//     BFSflag[i] = true;
//     order.push_back(i);
//
//     while (!que.empty()) {
//       now = que.front();
//       que.pop();
//
//       tmp.clear();
//       tmp = outNeighbours of now sorted by total degree
//
//       for (auto &t : tmp) {
//         if(BFSflag[t]) continue;
//         que.push(t);
//         BFSflag[t] = true;
//         order.push_back(t);
//       }
//     }
//   }
//
//   clear BFSflag and QueFlag;
//
//   retorder.resize(vsize);
//   retorder[order[i]] = visze - 1 - i;
// }


// -----------------------------------------------------------

// Graph::readGraph(const string &name) { // == create edge list + adj list
//   open file name
//   vector<edge> edges; edges.reserve(100 000 000);
//   int vsize;
//   while (line >> u >> v) {
//     edges.push_back(make_pair(u, v));
//     vsize = max(vsize, u, v);
//     edgenum ++;
//   }
//   graph.resize(vsize+1);
//
//   for each edge {
//     graph[first].outdegree ++
//     graph[second].indegree ++
//   }
//
//   cumulated degrees stored with outstart and instart
//   sort edges
//   outedge.resize(edgenum);
//   for each edge { add second to outedge }
// }
