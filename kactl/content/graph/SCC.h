/**
 * Author: Lukas Polacek
 * Date: 2009-10-28
 * License: CC0
 * Source: Czech graph algorithms book, by Demel. (Tarjan's algorithm)
 * Description: Finds strongly connected components in a
 * directed graph.
 * Time: O(E + V)
 * Status: Bruteforce-tested for N <= 5
 */
#include "../../stress-tests/utilities/template.h"
#include "graphs_structures.h"

template <typename T> vi find_scc(const digraph<T> &g, int &cnt) {
  digraph<T> g_rev = g.reverse();
  vi order;
  vector<bool> was(g.n, false);
  function<void(int)> dfs1 = [&](int v) {
    was[v] = true;
    for (int id : g.g[v]) {
      auto &e = g.edges[id];
      int to = e.to;
      if (!was[to]) {
        dfs1(to);
      }
    }
    order.push_back(v);
  };
  for (int i = 0; i < g.n; i++) {
    if (!was[i]) {
      dfs1(i);
    }
  }
  vector<int> c(g.n, -1);
  function<void(int)> dfs2 = [&](int v) {
    for (int id : g_rev.g[v]) {
      auto &e = g_rev.edges[id];
      int to = e.to;
      if (c[to] == -1) {
        c[to] = c[v];
        dfs2(to);
      }
    }
  };
  cnt = 0;
  for (int id = g.n - 1; id >= 0; id--) {
    int i = order[id];
    if (c[i] != -1) {
      continue;
    }
    c[i] = cnt++;
    dfs2(i);
  }
  return c;
  // c[i] <= c[j] for every edge i -> j
}