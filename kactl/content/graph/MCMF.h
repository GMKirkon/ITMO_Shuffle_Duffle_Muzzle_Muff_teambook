/**
* Author: Kirkon
* Date: 2024-12-12
* License: CC0
* Source: folklore
* Description: min cost max flow
* Time: O(flow * mlogn + mn)
* Status: stress-tested
*/
template <class F, class C = F> struct MCMF {
  struct Edge { int to; F flow, cap; C cost; };
  int n; vector<C> pot, dist;
  vector<int> previous_edge; vector<Edge> edges; vector<vector<int>> gr;
  
  MCMF(int n_) : n(n_) {
    pot.resize(n), dist.resize(n), previous_edge.resize(n), gr.resize(n);
  }

  void add(int u, int v, F cap, C cost) { assert(cap >= 0);
    gr[u].pb(edges.size()); edges.pb({v, 0, cap, cost}); gr[v].pb(edges.size()); edges.pb({u, 0, 0, -cost});
  }

  bool path(int s, int t) {
    constexpr C inf = numeric_limits<C>::max(); 
    dist.assign(n, inf);
    
    using T = pair<C, int>; priority_queue<T, vector<T>, greater<T>> bfs;
    bfs.push({dist[s] = 0, s});
    while (!bfs.empty()) {
    	auto [cur_dist, v] = bfs.top(); bfs.pop();
      if (cur_dist > dist[v]) continue;
      for (auto &e : gr[v]) {
        auto &E = edges[e]; if (E.flow < E.cap && ckmin(dist[E.to], cur_dist + E.cost + pot[v] - pot[E.to]))
          previous_edge[E.to] = e, bfs.push({dist[E.to], E.to});
      }
    }

    return dist[t] != inf;
  }

  pair<F, C> calc(int s, int t) {
    assert(s != t);
    rep(n) for (int e = 0; e < edges.size(); ++e) {
      const Edge &E = edges[e]; if (E.cap) ckmin(pot[E.to], pot[edges[e ^ 1].to] + E.cost);  // Bellman-Ford
    }
    F totalFlow = 0; C totalCost = 0;
    while (path(s, t)) {
      for (int i = 0; i < n; ++i) pot[i] += (dist[i] == numeric_limits<C>::max() ? 0 : dist[i]);
      F df = numeric_limits<F>::max();
      for (int x = t; x != s; x = edges[previous_edge[x] ^ 1].to) { const Edge &E = edges[previous_edge[x]]; ckmin(df, E.cap - E.flow); }
      totalFlow += df; totalCost += (pot[t] - pot[s]) * df;
      for (int x = t; x != s; x = edges[previous_edge[x] ^ 1].to) edges[previous_edge[x]].flow += df, edges[previous_edge[x] ^ 1].flow -= df;
    }
    return {totalFlow, totalCost};
  }
};
