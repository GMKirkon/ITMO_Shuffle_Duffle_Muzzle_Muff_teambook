/**
 * Author: Kirill Konovalov
 * Date: 2025-04-24
 * License: CC0
 * Source:
 * Description: 
 * Time: O(Vk + E)
 * Status: stress-tested, almost certainly is slower than dijkstra, but, for some corner cases its faster. Or if problem is bfs-specific.
 */
#pragma once
#include "../../stress-tests/utilities/template.h"
#include "graphs_structures.h"
vi zero_k_bfs(const graph<int>& g, int start, int k) {
  assert(0 <= start && start < g.n);
  int pw = 1; while (pw <= k) pw <<= 1;
  vi dist(g.n, std::numeric_limits<int>::max()); dist[start] = 0; vector<queue<int>> queues(pw); queues[0].push(start); int pointer = 0, cnt = 1;
  for (; cnt; ++pointer) while (!queues[pointer & (pw - 1)].empty()) {
      int v = queues[pointer & (pw - 1)].front(); queues[pointer & (pw - 1)].pop(); --cnt;
      if (dist[v] != pointer) continue;
      for (int id : g.g[v]) {
        auto& e = g.edges[id]; int to = e.from ^ e.to ^ v;
        if (dist[v] + e.cost < dist[to]) {
          dist[to] = dist[v] + e.cost; ++cnt;
          queues[(pointer + e.cost) & (pw - 1)].push(to);
        }
      }
    }
  return dist;
}
