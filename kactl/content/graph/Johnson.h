/**
 * Author: Kirill Konovalov
 * Date: 2025
 * License: CC0
 * Source: https://www.youtube.com/watch?v=0i_9WZDDikE
 * Description: calculates all-pairs shortest paths in a graph that might have negative edge weights.
 * Time: $O(N^3)$
 * Status: well-tested
 */
#pragma once
#include "../../stress-tests/utilities/template.h"
#include "../../content/graph/BellmanFord.h"
#include "../../content/graph/dijkstra.h"


vector<vector<ll>> johnson(graph<ll> &g) {
  int n = g.n;
  
  ++g.n; g.g.resize(g.n);
  for (int i = 0; i < n; ++i) g.add(n, i, 0);
  vector<ll> d = fordbellman(g, n);
  g.edges.erase(g.edges.end() - n, g.edges.end());
  for (auto& adj : g.g) while (!adj.empty() && adj.back() >= g.edges.size()) { adj.pop_back(); }
  --g.n; g.g.resize(g.n);

  for(auto& e : g.edges) e.cost += d[e.from] - d[e.to];
  vector<vector<ll>> ans(n, vector<ll>(n, numeric_limits<ll>::max()));
  for (int v = 0; v < n; ++v){
    ans[v] = dijkstra(g, v);
    for (int u = 0; u < n; ++u) if (ans[v][u] != numeric_limits<ll>::max()) ans[v][u] -= d[v] - d[u];
  }
  return ans;
}
