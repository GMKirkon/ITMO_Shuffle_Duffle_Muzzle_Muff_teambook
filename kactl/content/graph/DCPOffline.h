/**
 * Author: Andrey Matveev
 * Date: 2025-02-18
 * License: CC0
 * Source: https://codeforces.com/contest/2069/submission/306727520
 * Description: DCP offline algorithm. Actually could be generalized to any offline queries.
 * Time: $O(V \log^2(V))$
 * Status: Tested on https://codeforces.com/contest/2069/problem/F
 */
#pragma once
#include "../../stress-tests/utilities/template.h"

struct dsu_save {
    int v, rnkv, u, rnku;
    dsu_save() = default;
    dsu_save(int _v, int _rnkv, int _u, int _rnku) : v(_v), rnkv(_rnkv), u(_u), rnku(_rnku) {}
};

struct dsu_with_rollbacks {
    vector<int> p, rnk; int comps;
    stack<dsu_save> op;
    dsu_with_rollbacks() = default;
    dsu_with_rollbacks(int n) {
        p.resize(n); rnk.resize(n);
        iota(p.begin(), p.end(), 0); rnk.assign(n, 0); comps = n;
    }

    int find_set(int v) {
        return (v == p[v]) ? v : find_set(p[v]);
    }

    bool unite(int v, int u) {
        v = find_set(v); u = find_set(u);
        if (v == u) return false;
        comps--;
        if (rnk[v] > rnk[u]) swap(v, u);
        op.push(dsu_save(v, rnk[v], u, rnk[u])); p[v] = u;
        if (rnk[u] == rnk[v]) rnk[u]++;
        return true;
    }

    void rollback() {
        if (op.empty()) return;
        dsu_save x = op.top(); op.pop(); comps++;
        p[x.v] = x.v; rnk[x.v] = x.rnkv;
        p[x.u] = x.u; rnk[x.u] = x.rnku;
    }
};

struct query {
    int v, u;
    bool united = true;
    query(int _v, int _u) : v(_v), u(_u) {}
};

struct QueryTree {
    vector<vector<query>> t;
    dsu_with_rollbacks dsu;
    int T;

    QueryTree() = default;

    QueryTree(int _T, int n) : T(_T) {
        dsu = dsu_with_rollbacks(n);
        t.resize(4 * T + 4);
    }

    void add_to_tree(int v, int l, int r, int ul, int ur, query& q) {
        if (ul > ur) return;
        if (l == ul && r == ur) {
            t[v].push_back(q);
            return;
        }
        int mid = (l + r) / 2;
        add_to_tree(2 * v, l, mid, ul, min(ur, mid), q);
        add_to_tree(2 * v + 1, mid + 1, r, max(ul, mid + 1), ur, q);
    }

    void add_query(query q, int l, int r) { // edge (q.u, q.v) lives on segment [l, r]
        add_to_tree(1, 0, T - 1, l, r, q);
    }

    void dfs(int v, int l, int r, vector<int>& ans) {
        for (query& q : t[v]) {
            q.united = dsu.unite(q.v, q.u);
        }
        if (l == r) ans[l] = dsu.comps; // here you can customize answers on queries
        else { int mid = (l + r) / 2; dfs(2 * v, l, mid, ans); dfs(2 * v + 1, mid + 1, r, ans);}
        for (auto q : t[v]) {
            if (q.united) dsu.rollback();
        }
    }

    vector<int> solve() {
        vector<int> ans(T);
        dfs(1, 0, T - 1, ans);
        return ans;
    }
};