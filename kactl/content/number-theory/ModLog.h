/**
 * Author: Bjorn Martinsson
 * Date: 2020-06-03
 * License: CC0
 * Source: own work
 * Description: Returns the smallest $x > 0$ s.t. $a^x = b \pmod m$, or
 * $-1$ if no such $x$ exists. modLog(a,1,m) can be used to
 * calculate the order of $a$.
 * Time: $O(\sqrt m)$
 * Status: tested for all 0 <= a,x < 500 and 0 < m < 500.
 *
 * Details: This algorithm uses the baby-step giant-step method to
 * find (i,j) such that a^(n * i) = b * a^j (mod m), where n > sqrt(m)
 * and 0 < i, j <= n. If a and m are coprime then a^j has a modular
 * inverse, which means that a^(i * n - j) = b (mod m$).
 *
 * However this particular implementation of baby-step giant-step works even
 * without assuming a and m are coprime, using the following idea:
 *
 * Assume p^x is a prime divisor of m. Then we have 3 cases
 *	 1. b is divisible by p^x
 *	 2. b is divisible only by some p^y, 0<y<x
 *	 3. b is not divisible by p
 * The important thing to note is that in case 2, modLog(a,b,m) (if
 * it exists) cannot be > sqrt(m), (technically it cannot be >= log2(m)).
 * So once all exponenents of a that are <= sqrt(m) has been checked, you
 * cannot have case 2. Case 2 is the only tricky case.
 *
 * So the modification allowing for non-coprime input involves checking all
 * exponents of a that are <= n, and then handling the non-tricky cases by
 * a simple gcd(a^n,m) == gcd(b,m) check.
 */
#pragma once

#include "Factor.h"
#include "euclid.h"

ll modLog(ll a, ll b, ll m) {
	ll n = (ll) sqrt(m) + 1, e = 1, f = 1, j = 1;
	unordered_map<ll, ll> A;
	while (j <= n && (e = f = e * a % m) != b % m)
		A[e * b % m] = j++;
	if (e == b % m) return j;
	if (__gcd(m, e) == __gcd(m, b)) 
		rep(i,2,n+2) if (A.count(e = e * f % m))
			return n * i - A[e];
	return -1;
}

/**
 * Batch queries (a,b): smallest x >= 0 with a^x = b (mod m),
 * or -1. Requires m >= 2 with a primitive root and gcd(a,m)=1.
 * Unlike modLog, x=0 is allowed. Non-unit b has no solution.
 * After finding a primitive root, expected time is
 * O(sqrt(n * phi(m)) + n * log(m)), memory
 * O(min(phi(m), sqrt(n * phi(m)))) besides the answers.
 * Status: stress-tested.
 */
vi modLogs(const vector<pii>& q, int m) {
	int n = sz(q);
	if (!n) return {};
	assert(m >= 2);
	map<ul, int> f;
	factor_rec(m, f);
	assert(m == 2 || m == 4 ||
		(m % 4 && sz(f) - (int)f.count(2) == 1));
	int ph = m;
	for (auto p : f) ph -= ph / (int)p.first;
	f.clear();
	factor_rec(ph, f);

	int g = m == 2 ? 1 : 2;
	for (;; ++g) {
		if (__gcd(g, m) != 1) continue;
		bool ok = true;
		for (auto p : f)
			if (modPow(g, ph / p.first, m) == 1)
				ok = false;
		if (ok) break;
	}

	int k = max(1, (int)sqrt((long double)ph / n));
	int cnt = (ph - 1) / k + 1;
	unordered_map<int, int> mp;
	mp.max_load_factor(0.7f);
	mp.reserve(cnt + 1);
	ll step = (ll)modPow(g, k, m), cur = 1;
	rep(i,0,cnt+1) {
		mp[(int)cur] = i;
		cur = cur * step % m;
	}
	auto lg = [&](int x) {
		x %= m; if (x < 0) x += m;
		if (__gcd(x, m) != 1) return -1;
		ll cur = x;
		rep(j,0,k) {
			auto it = mp.find((int)cur);
			if (it != mp.end()) {
				ll z = ((ll)it->second * k - j) % ph;
				return (int)(z < 0 ? z + ph : z);
			}
			cur = cur * g % m;
		}
		return -1;
	};

	vi ans(n, -1);
	rep(i,0,n) {
		int A = lg(q[i].first), B = lg(q[i].second);
		assert(A != -1);
		if (B == -1) continue;
		ll x, y, d = euclid(A, ph, x, y);
		if (B % d) continue;
		ll mod = ph / d;
		ans[i] = (int)((B / d * x % mod + mod) % mod);
	}
	return ans;
}
