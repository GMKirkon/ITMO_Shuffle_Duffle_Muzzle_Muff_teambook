/**
 * Author: Simon Lindholm
 * Date: 2015-02-23
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/Bellman-Ford_algorithm
 * Description: Calculates shortest paths from $s$ in a graph that might have negative edge weights.
 * Unreachable nodes get dist = inf; nodes reachable through negative-weight cycles get dist = -inf.
 * Assumes $V^2 \max |w_i| < \tilde{} 2^{63}$.
 * Time: O(VE)
 * Status: Tested on kattis:shortestpath3
 */
#pragma once
#include "../../stress-tests/utilities/template.h"
#include "graphs_structures.h"

template <typename T> vector<T> fordbellman(const graph<T> &g, int start) {
  assert(0 <= start && start < g.n);
  constexpr T MAXVALUE = numeric_limits<T>::max(); vector<T> dist(g.n, MAXVALUE);
  dist[start] = 0; vector<int> pv(g.n, -1);
  auto relax = [&]() {
    for (int id = 0; id < g.edges.size(); ++id) {
      auto& e = g.edges[id];
      if (dist[e.from] < MAXVALUE) {
        if (dist[e.to] > dist[e.from] + e.cost) {
          dist[e.to] = dist[e.from] + e.cost;
          pv[e.to] = e.from;
        }
      }
    }
  };
 
  for (int i = 0; i + 1 < g.n; ++i) relax();
  return dist;
  /* find cycle with negative value
  auto cur = dist; relax();
  int broken = -1;
  for (int i = 0; i < g.n; ++i) {
    if (cur[i] != dist[i]) { broken = i; break; }
  }
  if (broken == -1) { return {}; }
	for (int i = 0; i < g.n; ++i) broken = pv[broken];
	vector<int> path{broken}; broken = pv[broken];
	while(broken != path[0]) {
		path.pb(broken);
		broken = pv[broken];
	}
	reverse(all(path));
	return path;
  */
}
