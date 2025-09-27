#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

mt19937 rnd(239);

//Solves "the number of distinct integers of the form ax+by for x,y>=0 and given a,b"

ll smart(ll a, ll b, ll c) {
  if (b < a) swap(a, b);
  if (c < 0) return 0ll;
  if (a == 0) {
    assert(0);
  }
  ll k = b / a;
  ll y_0 = c / b;
  ll x_0 = (c - y_0 * b) / a;
  ll cur_x = x_0;
  ll ret = (x_0 + 1) * (ll) (y_0 + 1);
  ll len = y_0 + 1;
  ret += len * (ll) (len - 1) / 2 * k;
  /*
  for (int y = y_0; y >= 0; y--) {
    ret += (cur_x + 1);
    cur_x += k;
  }
  */
  ll new_c = c + a * (ll) (-x_0 - 1 - k * (ll) y_0);
  ll new_b = b - a * (ll) k;
  return ret + smart(a, new_b, new_c);
}

long long FloorSumAP(long long a, long long b, long long c, long long n){
  if(!a) return (b / c) * (n + 1);
  if(a >= c or b >= c) return ( ( n * (n + 1) ) / 2) * (a / c) + (n + 1) * (b / c) + FloorSumAP(a % c, b % c, c, n);
  long long m = (a * n + b) / c;
  return m * n - FloorSumAP(c, c - b - 1, a, m - 1);
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
    ll a, b;
    cin >> a >> b;
    ll l, r;
    cin >> l >> r;
    ll g = __gcd(a, b);
    l = (l + g - 1) / g;
    r /= g;
    a /= g, b /= g;
    auto f = [&] (ll r) {
      ll sum = 0;
      ll lim = min(a - 1, r / b);
      r += a;
      r -= b * lim;
      return FloorSumAP(b, r, a, lim);
      /*
      for (int i = 0; i <= lim; i++) {
        sum += (r + b * (ll) i) / a;
      }
      return sum;
      */
    };
    if (l <= r) {
      cout << f(r) - f(l - 1) << '\n';
    } else {
      cout << 0 << '\n';
    }
  }
}
