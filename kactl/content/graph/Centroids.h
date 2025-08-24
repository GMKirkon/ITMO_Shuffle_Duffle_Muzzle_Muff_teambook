/**
 * Author: Tourist
 * Date: 2025
 * License: CC0
 * Source: github
 * Description: Finds centroids decomposition
 * Time: $O(n\log)$
 * Status: well-tested
 */
template <typename T>
vector<int> centroid_decomposition(const forest<T>& g) {
  int n = g.n;
  vector<bool> alive(n, true);
  vector<int> res; res.reserve(n);
 
  vector<int> sz(n);
  function<void(int, int)> Dfs = [&](int v, int pr) {
    sz[v] = 1;
    for (int eid : g.g[v]) {
      auto& e = g.edges[eid];
      int u = e.from ^ e.to ^ v;
      if (u != pr && alive[u]) {
        Dfs(u, v);
        sz[v] += sz[u];
      }
    }
  };
  function<void(int)> Build = [&](int v) -> void {
    Dfs(v, -1);
    int c = v;
    int pr = -1;
    while (true) {
      int nxt = -1;
      for (int eid : g.g[c]) {
        auto& e = g.edges[eid];
        int u = e.from ^ e.to ^ c;
        if (u != pr && alive[u] && 2 * sz[u] > sz[v]) {
          nxt = u;
          break;
        }
      }
      if (nxt == -1) {
        break;
      }
      pr = c;
      c = nxt;
    }
    res.pb(c);
    alive[c] = false;
    for (int eid : g.g[c]) {
      auto& e = g.edges[eid];
      int u = e.from ^ e.to ^ c;
      if (alive[u]) {
        Build(u);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (alive[i]) {
      Build(i);
    }
  }
  return res;
}

/*
  auto centers = centroid_decomposition(g);
  constexpr int LOGN = 17;
  vector<vector<array<int, 2>>> parents(LOGN, vector<array<int, 2>>(n));
  vector<int> alive(n, 1);
  vector<int> pointers(n, 0);
  { 
    vector<array<int, 2>> bfs(n);
    int head = 0, tail = 0;
    for (auto c : centers) {
      alive[c] = false;
      head = tail = 0;
      bfs[tail++] = {c, -1};
      while(head < tail) {
        auto [v, par] = bfs[head++];
        if (par != -1) {
          parents[pointers[v]][v] = parents[pointers[v]][par]; parents[pointers[v]][v][1]++;  ++pointers[v];  
        } else {
          parents[pointers[v]++][v] = {v, 0};
        }
        
        for (auto eid : g.g[v]) {
          auto& e = g.edges[eid];
          auto to = e.from ^ e.to ^ v;
          if (to == par || !alive[to]) {
            continue;
          }
          bfs[tail++] = {to, v};
        }
      }
    }
  }
*/