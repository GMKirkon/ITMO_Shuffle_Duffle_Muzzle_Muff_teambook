/**
 * Author: Kirill Konovalov
 * Date: 2025-04
 * License: CC0
 * Source: https://koosaga.com/307
 * Description: minkinetic Segment Tree, for finding the minimum value of a [a[i]*t + b[i]] on segments [l, r] with t monotonically increasing
 * Time: O(log^2 n) per query AMORTIZED, so no persistent version is possible
 * Status: stress-tested, tested on https://www.acmicpc.net/problem/12795
 */
#pragma once
#include "../../stress-tests/utilities/template.h"

constexpr int MAXT = 550000;
constexpr ll inf = numeric_limits<ll>::max() / 4;

struct line {
	ll A, B;
	ll /* __int128 */ eval(ll x) { return A * /* (__int128)(1) */ x + B; }
	ll cross_after(line &other, ll T) { // returns the x-intercept of intersection "strictly" larger than T
		if (other.A == A) { return inf; }
		ll up = other.B - B, dn = A - other.A;
		if (dn < 0) { dn *= -1; up *= -1; }
		ll incep = (up <= 0 ? -((-up) / dn) : (up + dn - 1) / dn);
		return incep > T ? incep : inf;
	}
};

struct kst {
	line tree[MAXT];
	ll melt[MAXT], T;
	int n;
	void pull(int p) {
		ll l = tree[p << 1].eval(T); ll r = tree[p << 1 ^ 1].eval(T);
		tree[p] = (l < r || (l == r && tree[p << 1].A < tree[p << 1 ^ 1].A)) ? tree[p << 1] : tree[p << 1 ^ 1];
		melt[p] = min({melt[p << 1], melt[p << 1 ^ 1], tree[p << 1].cross_after(tree[p << 1 ^ 1], T)});
	}
	void init(int s, int e, int p, vector<line> &l) {
		if (s == e) { tree[p] = l[s]; melt[p] = inf; return; }
		int m = (s + e) >> 1; init(s, m, p << 1, l); init(m + 1, e, p << 1 ^ 1, l);
    pull(p);
	}
	void update(int pos, line v, int s, int e, int p = 1) {
		if (s == e) { tree[p] = v; return; }
		int m = (s + e) >> 1;
		if (pos <= m) update(pos, v, s, m, p << 1);
		else update(pos, v, m + 1, e, p << 1 ^ 1);
		pull(p);
	}
	ll query(int s, int e, int ps, int pe, int p = 1) {
		if (e < ps || pe < s) return inf;
		if (s <= ps && pe <= e) return tree[p].eval(T);
		int pm = (ps + pe) >> 1;
		return min(query(s, e, ps, pm, p << 1), query(s, e, pm + 1, pe, p << 1 ^ 1));
	}
	void heaten(int s, int e, int p) {
		if (melt[p] > T) return;
		int m = (s + e) >> 1; heaten(s, m, p << 1); heaten(m + 1, e, p << 1 ^ 1);
		pull(p);
	}
	void init(vector<line> &l, ll _T) { n = sz(l); T = _T; init(0, n - 1, 1, l); }
  void heaten(ll _T) { assert(T <= _T); T = _T; heaten(0, n - 1, 1); }
} kst;

struct query {
  int s, e; ll t; int idx;
};
