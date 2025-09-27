#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

mt19937 rnd(239);

ll gcd (ll a, ll b, ll & x, ll & y) {
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}
	ll x1, y1;
	ll d = gcd (b%a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}

int main() {
#ifdef ONPC
  freopen("a.in", "r", stdin);
#endif
  ios::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    int n, m, h;
    cin >> n >> m >> h;
    vector <int> l(n);
    for (int i = 0; i < n; i++) cin >> l[i];
    vector <int> s(n);
    for (int i = 0; i < n; i++) cin >> s[i];
    vector <vector <int> > g(n);
    for (int i = 0; i < m; i++) {
      int a, b;
      cin >> a >> b;
      a--, b--;
      g[a].push_back(b);
      g[b].push_back(a);
    }
    set <pair <ll, int> > q;
    ll inf = 1e18;
    vector <ll> d(n, inf);
    d[0] = 0;
    q.insert({d[0], 0});
    auto w = [&] (int v, int to) {
      ll a = (l[to] - l[v] + h) % h;
      ll b = (s[v] - s[to] + h) % h;
      ll g = __gcd(b, (ll) h);
      if (a % g != 0) return inf;
      ll ta = a / g;
      ll tb = b / g;
      ll th = h / g;
      ll x, y;
      gcd(th, tb, x, y);
      //(x * th) + (y * tb) == 1
      ll rev = (ta * y % th) % th;
      if (rev < 0) rev += th;
      if (rev < d[v]) {
        rev += ((d[v] - rev) / th) * th;
      }
      while (rev < d[v]) rev += th;
      /*
      for (ll j = d[v] + 1; j <= d[v] + h; j++) {
        if (ta == (j * tb) % th) {
          return j;
        }
      }
      */
      return rev;
    };
    while (!q.empty()) {
      int v = q.begin()->second;
      q.erase(q.begin());
      for (int x : g[v]) {
        if (d[x] > w(v, x) + 1) {
          q.erase({d[x], x});
          d[x] = w(v, x) + 1;
          q.insert({d[x], x});
        }
      }
    }
    if (d[n - 1] == inf) {
      cout << -1 << '\n';
    } else {
      cout << d[n - 1] << '\n';
    }
  }
}
