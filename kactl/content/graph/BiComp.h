/**
 * Author: Tourist
 * Date: 2024-12-12
 * License: CC0
 * Source: folklore
 * Description: Finds all biconnected components in an undirected graph
 * Time: O(E + V)
 * Status: tested by Tourist
 */

template <typename T> vector<int> find_edge_biconnected(dfs_undigraph<T> &g, int &cnt) {
  g.dfs_all();
  vector<int> vertex_comp(g.n);
  cnt = 0;
  for (int i : g.order) {
    if (g.pv[i] == -1 || g.min_depth[i] == g.depth[i]) {
      vertex_comp[i] = cnt++;
    } else {
      vertex_comp[i] = vertex_comp[g.pv[i]];
    }
  }
  return vertex_comp;
}

template <typename T> vector<int> find_vertex_biconnected(dfs_undigraph<T> &g, int &cnt) {
  g.dfs_all();
  vector<int> vertex_comp(g.n);
  cnt = 0;
  for (int i : g.order) {
    if (g.pv[i] == -1) {
      vertex_comp[i] = -1;
      continue;
    }
    if (g.min_depth[i] >= g.depth[g.pv[i]]) {
      vertex_comp[i] = cnt++;
    } else {
      vertex_comp[i] = vertex_comp[g.pv[i]];
    }
  }
  vector<int> edge_comp(g.edges.size(), -1);
  for (int id = 0; id < (int)g.edges.size(); id++) {
    if (g.ignore != nullptr && g.ignore(id)) {
      continue;
    }
    int x = g.edges[id].from;
    int y = g.edges[id].to;
    int z = (g.depth[x] > g.depth[y] ? x : y);
    edge_comp[id] = vertex_comp[z];
  }
  return edge_comp;
}

template <typename T> vector<bool> find_bridges(dfs_undigraph<T> &g) {
  g.dfs_all();
  vector<bool> bridge(g.edges.size(), false);
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] != -1 && g.min_depth[i] == g.depth[i]) {
      bridge[g.pe[i]] = true;
    }
  }
  return bridge;
}

template <typename T> vector<bool> find_cutpoints(dfs_undigraph<T> &g) {
  g.dfs_all();
  vector<bool> cutpoint(g.n, false);
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] != -1 && g.min_depth[i] >= g.depth[g.pv[i]]) {
      cutpoint[g.pv[i]] = true;
    }
  }
  vector<int> children(g.n, 0);
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] != -1) {
      children[g.pv[i]]++;
    }
  }
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] == -1 && children[i] < 2) {
      cutpoint[i] = false;
    }
  }
  return cutpoint;
}
