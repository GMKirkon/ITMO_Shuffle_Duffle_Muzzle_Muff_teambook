/**
 * Author: tourist
 * Date: 2025
 * License: CC0
 * Source: https://github.com/the-tourist/algo/blob/master/graph/dijkstra.cpp
 * Description: Calculates shortest paths from $s$ in a graph that might NOT have negative edge weights.
 * Time: $O(N\log N)$
 * Status: well-tested
 */
#pragma once
#include "../../stress-tests/utilities/template.h"
#include "graphs_structures.h"


template <typename T> vector<T> dijkstra(const graph<T> &g, int start) {
  assert(0 <= start && start < g.n);
  constexpr T MAXVALUE = numeric_limits<T>::max(); vector<T> dist(g.n, MAXVALUE);
  priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> s; dist[start] = 0; s.emplace(dist[start], start);
  while (!s.empty()) {
    T expected = s.top().first; int i = s.top().second; s.pop();
    if (dist[i] != expected) continue;
    for (int id : g.g[i]) {
      auto &e = g.edges[id]; int to = e.from ^ e.to ^ i;
      if (dist[i] + e.cost < dist[to]) {
        dist[to] = dist[i] + e.cost; s.emplace(dist[to], to);
      }
    }
  }
  return dist;
}  