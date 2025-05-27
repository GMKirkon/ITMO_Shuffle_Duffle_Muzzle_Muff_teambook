#pragma once
#include "../../stress-tests/utilities/template.h"

#ifndef KACTL_GRAPH_STRUCTURES_H
#define KACTL_GRAPH_STRUCTURES_H

template <typename T> class graph {
public:
  struct edge {
    int from, to; T cost;
  };
  vector<edge> edges;
  vvi g;
  int n;
  graph(int _n) : n(_n) {
    g.resize(n);
  }
  virtual int add(int from, int to, T cost = 1) = 0;
  void read(const int m, const bool is_weighted) {
    for (int i = 0; i < m; ++i) {
      int u, v; T w = 1; cin >> u >> v; if (is_weighted) cin >> w; this->add(--u, --v, w);
    }
  }
};

template <typename T> class undigraph : public graph<T> {
public:
  using graph<T>::edges; using graph<T>::g; using graph<T>::n;
  undigraph(int _n) : graph<T>(_n) {}
  int add(int from, int to, T cost = 1) override {
    assert(0 <= from && from < n && 0 <= to && to < n);
    int id = (int)edges.size(); g[from].push_back(id); g[to].push_back(id); edges.push_back({from, to, cost});
    return id;
  }
};

template <typename T> class dfs_undigraph : public undigraph<T> {
public:
  using undigraph<T>::edges; using undigraph<T>::g; using undigraph<T>::n;
  vi pv, pe, order, pos, end, sz, root, depth, min_depth, dist, was;
  int attempt;

  dfs_undigraph(int _n) : undigraph<T>(_n) {}

  void init() {
    pv = vi(n, -1); pe = vi(n, -1); pos = vi(n, -1); end = vi(n, -1); sz = vi(n, 0);
    root = vi(n, -1); depth = vi(n, -1); min_depth = vi(n, -1);  was = vi(n, -1);
    order.clear(); dist = vector<T>(n);
    attempt = 0;
  }

  void clear() {
    pv.clear(); pe.clear(); order.clear(); pos.clear(); end.clear(); sz.clear(); 
    root.clear(); depth.clear(); min_depth.clear(); dist.clear(); was.clear();
  }

private:
  void do_dfs(int v) {
    sz[v] = 1; was[v] = attempt; pos[v] = (int)order.size(); order.push_back(v); min_depth[v] = depth[v];
    for (int id : g[v]) {
      if (id == pe[v]) continue;
      auto &e = edges[id];
      int to = e.from ^ e.to ^ v;
      if (was[to] == attempt) {
        min_depth[v] = min(min_depth[v], depth[to]);
        continue;
      }
      depth[to] = depth[v] + 1;
      dist[to] = dist[v] + e.cost;
      pv[to] = v;
      pe[to] = id;
      root[to] = (root[v] != -1 ? root[v] : to);
      do_dfs(to);
      sz[v] += sz[to];
      min_depth[v] = min(min_depth[v], min_depth[to]);
    }
    end[v] = (int)order.size() - 1;
  }

  void do_dfs_from(int v) {
    ++attempt; depth[v] = 0; dist[v] = T{}; root[v] = v; pv[v] = pe[v] = -1;
    do_dfs(v);
  }

public:
  void dfs(int v, bool clear_order = true) {
    if (pv.empty()) {
      init();
    } else {
      if (clear_order) {
        order.clear();
      }
    }
    do_dfs_from(v);
  }

  void dfs_all() {
    init();
    for (int v = 0; v < n; v++) {
      if (depth[v] == -1) {
        do_dfs_from(v);
      }
    }
    assert((int)order.size() == n);
  }
};


template <typename T> class digraph : public graph<T> {
public:
  using graph<T>::edges; using graph<T>::g; using graph<T>::n;
  digraph(int _n) : graph<T>(_n) {}

  int add(int from, int to, T cost = 1) override {
    assert(0 <= from && from < n && 0 <= to && to < n);
    int id = (int)edges.size(); g[from].push_back(id); edges.push_back({from, to, cost});
    return id;
  }

  digraph<T> reverse() const {
    digraph<T> rev(n);
    for (auto &e : edges) {
      rev.add(e.to, e.from, e.cost);
    }
    return rev;
  }
};

template <typename T> class dfs_digraph : public digraph<T> {
public:
  using digraph<T>::edges; using digraph<T>::g; using digraph<T>::n;

  vi pv, pe, order, pos, end, sz, root, depth, dist;

  dfs_digraph(int _n) : digraph<T>(_n) {}

  void clear() {
    pv.clear(); pe.clear(); order.clear();  pos.clear(); end.clear(); sz.clear(); root.clear(); depth.clear(); dist.clear();
  }

  void init() {
    pv = vi(n, -1); pe = vi(n, -1); pos = vi(n, -1); end = vi(n, -1); sz = vi(n, 0); root = vi(n, -1); depth = vi(n, -1);
    order.clear();
    dist = vector<T>(n);
  }

private:
  void do_dfs(int v) {
    sz[v] = 1; pos[v] = (int)order.size(); order.push_back(v);
    for (int id : g[v]) {
      if (id == pe[v]) continue;
      auto &e = edges[id];
      int to = e.from ^ e.to ^ v;
      if (depth[to] != -1) continue;
      depth[to] = depth[v] + 1;
      dist[to] = dist[v] + e.cost;
      pv[to] = v;
      pe[to] = id;
      root[to] = (root[v] != -1 ? root[v] : to);
      do_dfs(to);
      sz[v] += sz[to];
    }
    end[v] = (int)order.size() - 1;
  }

  void do_dfs_from(int v) {
    depth[v] = 0; dist[v] = T{}; root[v] = v; pv[v] = pe[v] = -1;
    do_dfs(v);
  }

public:
  int dfs_one_unsafe(int v) {
    // run init() before this
    // then run this with the required v's
    do_dfs_from(v);
    return v;
  }

  int dfs(int v) {
    init();
    do_dfs_from(v);
    //    assert((int) order.size() == n);
    return v;
  }

  void dfs_many(const vector<int> &roots) {
    init();
    for (int v : roots) {
      if (depth[v] == -1) {
        do_dfs_from(v);
      }
    }
    //    assert((int) order.size() == n);
  }

  vector<int> dfs_all() {
    init();
    vector<int> roots;
    for (int v = 0; v < n; v++) {
      if (depth[v] == -1) {
        roots.push_back(v);
        do_dfs_from(v);
      }
    }
    assert((int)order.size() == n);
    return roots;
  }
};
#endif