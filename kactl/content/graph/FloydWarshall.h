/**
 * Author: Kirill Konovalov
 * Date: 2025
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/Floyd–Warshall_algorithm
 * Description: Calculates all-pairs shortest path in a directed graph that might have negative edge weights.
 * Input is an distance matrix $m$, where $m[i][j] = \texttt{inf}$ if $i$ and $j$ are not adjacent.
 * As output, $m[i][j]$ is set to the shortest distance between $i$ and $j$, \texttt{inf} if no path, or \texttt{-inf} if the path goes through a negative-weight cycle.
 * Time: O(N^3)
 * Status: stress-tested
 */
#pragma once
#include "../../stress-tests/utilities/template.h"
#include "../../content/graph/graphs_structures.h"

vvl floydWarshall(graph<ll> &g) {
	static constexpr ll inf = numeric_limits<ll>::max() / 4;
	int n = g.n; vvl ans(n, vl(n, inf)); for (int i = 0; i < n; i++) ans[i][i] = 0;
	for (auto& e : g.edges) ans[e.from][e.to] = min(ans[e.from][e.to], e.cost);
	rep(k, 0, n) rep(i, 0, n) rep(j, 0, n) if (ans[i][k] != inf && ans[k][j] != inf) {
		ans[i][j] = min(ans[i][j], ans[i][k] + ans[k][j]);
	}
	return ans;
}
