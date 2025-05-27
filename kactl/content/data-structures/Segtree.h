/**
 * Author: folklore codeforces
 * Date: 2025-05
 * License: CC0
 * Source: https://codeforces.com/blog/entry/18051
 * Time: O(\log N)
 * Description: tree for += ops and sum
 * Status: works very good
 */
#pragma once

struct segtree {
    static constexpr int N = 100'000;
    int n;  // array size
    int t[2 * N];
    void build() {
        for (int i = n - 1; i > 0; --i) t[i] = t[i << 1] + t[i << 1 | 1];
    }
    void modify(int p, int value) {  // set value at position p
      for (t[p += n] = value; p > 1; p >>= 1) t[p>>1] = t[p] + t[p^1];
    }
    int query(int l, int r) {  // sum on interval [l, r)
        int res = 0;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l&1) res += t[l++];
            if (r&1) res += t[--r];
        }
        return res;
    }
    int query(int p) {
        int res = 0;
        for (p += n; p > 0; p >>= 1) res += t[p];
        return res;
    }
    void push() {
        for (int i = 1; i < n; ++i) {
            t[i << 1] += t[i];
            t[i << 1 | 1] += t[i];
            t[i] = 0;
        }
    }
};
