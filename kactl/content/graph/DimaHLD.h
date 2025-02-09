/**
 * Author: dimms
 * Date: 2024
 * License: CC0
 * Source: telegram
 * Description: Builds HLD
 * Time: O(log^2 for query) probably O(log)
 * Status: not tested
 */


struct segtree { }

struct HLD {
    int n;
    vector<int> par, val, vale, tin, tout, sz, top;
    vector<vector<int>> up;
    int LG;
    segtree tree_v, tree_e;

    HLD() { }

    HLD(vector<vector<int>> &G) {
        n = G.size();
        par.resize(n);
        val.resize(n, 1);
        vale.resize(n, 1);
        tin.resize(n);
        tout.resize(n);
        sz.resize(n);
        top.resize(n);
        dfs(0, G);
        dfs2(0, G);
        tree_v = segtree(n);
        tree_e = segtree(n);

        LG = 0;
        while (1 << LG < n)
            LG++;
        up.resize(LG + 1, vector<int>(n));
        up[0] = par;
        for (int i = 1; i <= LG; i++) {
            for (int j = 0; j < n; j++)
                up[i][j] = up[i - 1][up[i - 1][j]];
        }
    }

    void dfs(int v, vector<vector<int>> &G, int p = -1) {
        if (p != -1)
            G[v].erase(find(G[v].begin(), G[v].end(), p));
        par[v] = p != -1 ? p : v;
        sz[v] = 1;
        for (int to : G[v]) {
            dfs(to, G, v);
            sz[v] += sz[to];
        }
        sort(G[v].begin(), G[v].end(), [&](int x, int y) {
            return sz[x] > sz[y];
        });
    }

    void dfs2(int v, vector<vector<int>> &G) {
        static int timer = 0;
        tin[v] = timer++;
        if (!G[v].empty())
            top[G[v][0]] = top[v];
        for (int i = 0; i < G[v].size(); i++) {
            int u = G[v][i];
            if (i)
                top[u] = u;
            dfs2(u, G);
        }
        tout[v] = timer;
    }

    bool isPar(int v, int u) {
        return tin[v] <= tin[u] && tout[v] >= tout[u];
    }

    int lca(int v, int u) {
        if (isPar(v, u))
            return v;
        for (int i = LG; i >= 0; i--) {
            if (!isPar(up[i][v], u))
                v = up[i][v];
        }
        return up[0][v];
    }

    vector<pair<int, int>> get_e_vert_path(int v, int l) {
        vector<pair<int, int>> res;
        res.reserve(LG * 2 + 3);
        while (true) {
            int x = top[v];
            if (isPar(x, l))
                break;
            res.push_back({tin[x], tin[v] + 1});
            v = par[x];
        }
        res.push_back({tin[l] + 1, tin[v] + 1});
        return res;
    }

    vector<pair<int, int>> get_e_path(int v, int u) {
        int l = lca(v, u);
        vector<pair<int, int>> res = get_e_vert_path(v, l);
        for (auto elem : get_e_vert_path(u, l))
            res.push_back(elem);
        return res;
    }

    vector<pair<int, int>> get_v_path(int v, int u) {
        auto res = get_e_path(v, u);
        int l = tin[lca(v, u)];
        res.push_back({l, l + 1});
        return res;
    }

    void mul_v(int v, int u, int delta) {
        for (auto [l, r] : get_v_path(v, u)) {
            tree_v.update(l, r, delta);
        }
    }

    void mul_e(int v, int u, int delta) {
        for (auto [l, r] : get_e_path(v, u))
            tree_e.update(l, r, delta);
    }

    int get_v_sum(int v, int u) {
        ll res = 0;
        for (auto [l, r] : get_v_path(v, u))
            res += tree_v.get_sum(l, r);
        return res % mod;
    }

    int get_e_sum(int v, int u) {
        ll res = 0;
        for (auto [l, r] : get_e_path(v, u))
            res += tree_e.get_sum(l, r);
        return res % mod;
    }
};

