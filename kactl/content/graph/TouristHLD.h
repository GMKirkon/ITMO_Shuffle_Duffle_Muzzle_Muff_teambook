/**
 * Author: Tourist
 * Date: 2025
 * License: CC0
 * Source: github
 * Description: Builds HLD
 * Time: $O(\log^2)$ probably actually $O(\log)$
 * Status: well-tested
 */
#pragma once
  template <typename T>
  class digraph : public graph<T> {
public:
    using graph<T>::edges;
    using graph<T>::g;
    using graph<T>::n;

    digraph(int _n) : graph<T>(_n) {}

    int add(int from, int to, T cost = 1) override {
      assert(0 <= from && from < n && 0 <= to && to < n);
      int id = (int)edges.size();
      g[from].push_back(id);
      edges.push_back({from, to, cost});
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

  template <typename T>
  class dfs_digraph : public digraph<T> {
public:
    using digraph<T>::edges;
    using digraph<T>::g;
    using digraph<T>::n;

    vector<int> pv;
    vector<int> pe;
    vector<int> order;
    vector<int> pos;
    vector<int> end;
    vector<int> sz;
    vector<int> root;
    vector<int> depth;
    vector<T> dist;

    dfs_digraph(int _n) : digraph<T>(_n) {}

    void clear() {
      pv.clear();
      pe.clear();
      order.clear();
      pos.clear();
      end.clear();
      sz.clear();
      root.clear();
      depth.clear();
      dist.clear();
    }

    void init() {
      pv = vector<int>(n, -1);
      pe = vector<int>(n, -1);
      order.clear();
      pos = vector<int>(n, -1);
      end = vector<int>(n, -1);
      sz = vector<int>(n, 0);
      root = vector<int>(n, -1);
      depth = vector<int>(n, -1);
      dist = vector<T>(n);
    }

private:
    void do_dfs(int v) {
      pos[v] = (int)order.size();
      order.push_back(v);
      sz[v] = 1;
      for (int id : g[v]) {
        if (id == pe[v]) {
          continue;
        }
        auto &e = edges[id];
        int to = e.from ^ e.to ^ v;
        // well, this is controversial...
        if (depth[to] != -1) {
          continue;
        }
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
      depth[v] = 0;
      dist[v] = T{};
      root[v] = v;
      pv[v] = pe[v] = -1;
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

  template <typename T>
  class forest : public graph<T> {
public:
    using graph<T>::edges;
    using graph<T>::g;
    using graph<T>::n;

    forest(int _n) : graph<T>(_n) {
    }

    int add(int from, int to, T cost = 1) {
      assert(0 <= from && from < n && 0 <= to && to < n);
      int id = (int) edges.size();
      assert(id < n - 1);
      g[from].push_back(id);
      g[to].push_back(id);
      edges.push_back({from, to, cost});
      return id;
    }
  };

  template <typename T>
  class dfs_forest : public forest<T> {
public:
    using forest<T>::edges;
    using forest<T>::g;
    using forest<T>::n;

    vector<int> pv;
    vector<int> pe;
    vector<int> order;
    vector<int> pos;
    vector<int> end;
    vector<int> sz;
    vector<int> root;
    vector<int> depth;
    vector<T> dist;

    dfs_forest(int _n) : forest<T>(_n) {
    }

    void init() {
      pv = vector<int>(n, -1);
      pe = vector<int>(n, -1);
      order.clear();
      pos = vector<int>(n, -1);
      end = vector<int>(n, -1);
      sz = vector<int>(n, 0);
      root = vector<int>(n, -1);
      depth = vector<int>(n, -1);
      dist = vector<T>(n);
    }

    void clear() {
      pv.clear();
      pe.clear();
      order.clear();
      pos.clear();
      end.clear();
      sz.clear();
      root.clear();
      depth.clear();
      dist.clear();
    }

private:
    void do_dfs(int v) {
      pos[v] = (int) order.size();
      order.push_back(v);
      sz[v] = 1;
      for (int id : g[v]) {
        if (id == pe[v]) {
          continue;
        }
        auto [e_from, e_to, cost] = edges[id];
        int to = e_from ^ e_to ^ v;
        depth[to] = depth[v] + 1;
        dist[to] = dist[v] + cost;
        pv[to] = v;
        pe[to] = id;
        root[to] = (root[v] != -1 ? root[v] : to);
        do_dfs(to);
        sz[v] += sz[to];
      }
      end[v] = (int) order.size() - 1;
    }

    void do_dfs_from(int v) {
      depth[v] = 0;
      dist[v] = T{};
      root[v] = v;
      pv[v] = pe[v] = -1;
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
      assert((int) order.size() == n);
    }
  };

  template <typename T>
  class hld_forest : public dfs_forest<T> {
public:
    using dfs_forest<T>::edges;
    using dfs_forest<T>::g;
    using dfs_forest<T>::n;
    using dfs_forest<T>::pv;
    using dfs_forest<T>::sz;
    using dfs_forest<T>::root;
    using dfs_forest<T>::pos;
    using dfs_forest<T>::end;
    using dfs_forest<T>::order;
    using dfs_forest<T>::depth;
    using dfs_forest<T>::dfs;
    using dfs_forest<T>::dfs_all;

    vector<int> head;
    vector<int> visited;

    hld_forest(int _n) : dfs_forest<T>(_n) {
      visited.resize(n);
    }

    void build_hld(const vector<int> &roots) {
      for (int tries = 0; tries < 2; tries++) {
        if (roots.empty()) {
          dfs_all();
        } else {
          order.clear();
          for (int root : roots) {
            dfs(root, false);
          }
          assert((int) order.size() == n);
        }
        if (tries == 1) {
          break;
        }
        for (int i = 0; i < n; ++i) {
          if (g[i].empty()) {
            continue;
          }
          int best = -1, bid = 0;
          for (int j = 0; j < (int) g[i].size(); ++j) {
            int id = g[i][j];
            auto [from, to, cost] = edges[id];
            int v = from ^ to ^ i;
            if (pv[v] != i) {
              continue;
            }
            if (sz[v] > best) {
              best = sz[v];
              bid = j;
            }
          }
          swap(g[i][0], g[i][bid]);
        }
      }
      head.resize(n);
      iota(head.begin(), head.end(), 0);
      for (int i = 0; i + 1 < n; ++i) {
        int x = order[i];
        int y = order[i + 1];
        if (pv[y] == x) {
          head[y] = head[x];
        }
      }
    }

    void build_hld(int v) {
      build_hld(vector<int>{v});
    }

    void build_hld_all() {
      build_hld(vector<int>());
    }

    bool apply_on_path(int x, int y, bool with_lca, function<void(int,int,bool)> f) {
      // f(x, y, up): up -- whether this part of the path goes up
      assert(!head.empty());
      int z = lca(x, y);
      if (z == -1) {
        return false;
      }
      {
        int v = x;
        while (v != z) {
          if (depth[head[v]] <= depth[z]) {
            f(pos[z] + 1, pos[v], true);
            break;
          }
          f(pos[head[v]], pos[v], true);
          v = pv[head[v]];
        }
      }
      if (with_lca) {
        f(pos[z], pos[z], false);
      }
      {
        int v = y;
        int cnt_visited = 0;
        while (v != z) {
          if (depth[head[v]] <= depth[z]) {
            f(pos[z] + 1, pos[v], false);
            break;
          }
          visited[cnt_visited++] = v;
          v = pv[head[v]];
        }
        for (int at = cnt_visited - 1; at >= 0; at--) {
          v = visited[at];
          f(pos[head[v]], pos[v], false);
        }
      }
      return true;
    }

    bool anc(int x, int y) {
      return (pos[x] <= pos[y] && end[y] <= end[x]);
    }

    int go_up(int x, int up) {
      int target = depth[x] - up;
      if (target < 0) {
        return -1;
      }
      while (depth[head[x]] > target) {
        x = pv[head[x]];
      }
      return order[pos[x] - depth[x] + target];
    }

    int lca(int x, int y) {
      if (root[x] != root[y]) {
        return -1;
      }
      for (; head[x] != head[y]; y = pv[head[y]]) {
        if (depth[head[x]] > depth[head[y]]) {
          swap(x, y);
        }
      }
      return depth[x] < depth[y] ? x : y;
    }
  };
