/**
 * Date: 2026-10-04
 * License: CC0
 * Source: https://algs4.cs.princeton.edu/44sp/BellmanFordSP.java.html
 * Description: Calculates shortest paths from start with SPFA.
 * Unreachable vertices get numeric_limits<T>::max().
 * If a reachable negative cycle exists, returns {} and sets cycle
 * to its vertices in edge order, without repeating the first.
 * Otherwise cycle is empty. Requires integer weights and no overflow.
 * To search the whole graph, add a source with zero edges to all vertices.
 * Time: O(VE), extra space O(V).
 * Status: stress-tested
 */
#pragma once
#include "graphs_structures.h"

template <typename T> vector<T> spfa(const graph<T> &g, int start, vector<int> &cycle) {
  assert(0 <= start && start < g.n);
  constexpr T MAXVALUE = numeric_limits<T>::max(); vector<T> dist(g.n, MAXVALUE);
  vector<int> pv(g.n, -1), state(g.n); vector<bool> inq(g.n);
  cycle.clear();
  auto find_cycle = [&]() -> vector<int> {
    fill(all(state), 0);
    for (int s = 0; s < g.n; ++s) {
      if (state[s]) continue;
      int v = s;
      while (v != -1 && state[v] == 0) {
        state[v] = 1; v = pv[v];
      }
      if (v != -1 && state[v] == 1) {
        vector<int> path{v};
        for (int u = pv[v]; u != v; u = pv[u]) path.push_back(u);
        reverse(all(path));
        return path;
      }
      v = s;
      while (v != -1 && state[v] == 1) {
        state[v] = 2; v = pv[v];
      }
    }
    return {};
  };

  queue<int> q; dist[start] = 0; q.push(start); inq[start] = true;
  int scanned = 0;
  while (!q.empty()) {
    int v = q.front(); q.pop(); inq[v] = false;
    for (int id : g.g[v]) {
      auto &e = g.edges[id]; int to = e.from ^ e.to ^ v;
      if (dist[v] + e.cost < dist[to]) {
        dist[to] = dist[v] + e.cost; pv[to] = v;
        if (!inq[to]) { inq[to] = true; q.push(to); }
      }
      // Every V examined edges: O(V) to find a predecessor cycle.
      if (++scanned == g.n) {
        scanned = 0; cycle = find_cycle();
        if (!cycle.empty()) return {};
      }
    }
  }
  return dist;
}
