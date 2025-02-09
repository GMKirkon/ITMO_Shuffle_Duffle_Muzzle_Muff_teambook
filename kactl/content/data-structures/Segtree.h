/**
 * Author: Dmitry Ivchenko
 * Date: 2024-12-12
 * License: CC0
 * Source: folklore
 * Description: tree for mx and += ops
 * Time: O(\log N)
 * Status: unknown
 */
#pragma once

struct segtree {
    int n;
    vector<ll> mx, upd;

    segtree() { }

    segtree(int n): n(n) {
        mx.resize(n * 4);
        upd.resize(n * 4);
    }

    void add(int v, int vl, int vr, int l, int r, ll x) {
        if (vr <= l || vl >= r)
            return;
        if (l <= vl && vr <= r) {
            mx[v] += x;
            upd[v] += x;
            return;
        }
        int vm = (vl + vr) >> 1;
        add(v * 2, vl, vm, l, r, x);
        add(v * 2 + 1, vm, vr, l, r, x);
        mx[v] = max(mx[v * 2], mx[v * 2 + 1]) + upd[v];
    }

    void add(int l, int r, ll x) {
        add(1, 0, n, l, r, x);
    }

    ll getMax(int v, int vl, int vr, int l, int r) {
        if (vr <= l || vl >= r)
            return INT64_MIN;
        if (l <= vl && vr <= r)
            return mx[v];
        int vm = (vl + vr) >> 1;
        return max(getMax(v * 2, vl, vm, l, r), getMax(v * 2 + 1, vm, vr, l, r)) + upd[v];
    }

    ll getMax(int l, int r) {
        return getMax(1, 0, n, l, r);
    }

    ll getMax() {
        return mx[1];
    }
};
