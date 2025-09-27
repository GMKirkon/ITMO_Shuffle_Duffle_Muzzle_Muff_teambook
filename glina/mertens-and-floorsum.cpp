#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>

using namespace std;

typedef __int128 ll;

mt19937 rnd(239);

const int mod = 998244353;

//FUNCTION SMART -> FASTER NAIVE AND
// FAST M(N) = sum_{n<=N} mu(n)
//USEFUL FOR MOBIOUS INVERSION 

int naive(int a, int b, int c) {
  int ans = 0;
  for (int x = 0; a * x <= c; x++) {
    for (int y = 0; a * x + b * y <= c; y++) {
      ans++;
    }
  }
  return ans;
}

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


const int N = 1e7 + 20;

bool prime[N];
signed char get_mu[N];

ll sum[N];

map <ll, ll> M_map;

ll M(ll n) {
  if (n < N) return sum[n];
  if (M_map.count(n)) return M_map[n];
  if (n == 0) return 0ll;
  ll ans = 1;
  ll l = 2;
  while (l <= n) {
    ll r = n / (n / l);
    ans -= M(n / l) * (r - l + 1);
    l = r + 1;
  }
  return M_map[n] = ans;
}

vector <pair <ll, int> > factorize(ll n) {
  vector <pair <ll, int> > ans;
  for (ll i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      int deg = 0;
      while (n % i == 0) {
        n /= i;
        deg++;
      }
      ans.push_back({i, deg});
    }
  }
  if (n != 1) ans.push_back({n, 1});
  return ans;
}

ll mu(ll n) {
  return get_mu[n];
  auto a = factorize(n);
  int sign = 1;
  for (auto c : a) {
    if (c.second > 1) {
      return 0ll;
    }
    sign *= -1;
  }
  return sign;
}

ll solve_atmost(ll n, ll a, ll b) {
  ll x = b;
  ll y = a;
  ll c = n * (ll) a;
  ll result = smart(x, y, c);
  result -= n + 1;
  return result;
}

ll solve_less(ll n, ll a, ll b) {
  ll result = solve_atmost(n, a, b) - (n / b);
  return result;
}

ll solve_atmost_coprime(ll n, ll a, ll b) {
  ll ans = 0;
  ll l = 1;
  while (l <= n) {
    ll r = n / (n / l);
    ans += (M(r) - M(l - 1)) * solve_atmost(n / l, a, b);
    l = r + 1;
  }
  return ans;
}

ll solve_less_coprime(ll n, ll a, ll b) {
  ll ans = 0;
  ll l = 1;
  while (l <= n) {
    ll r = n / (n / l);
    ans += (M(r) - M(l - 1)) * solve_less(n / l, a, b);
    l = r + 1;
  }
  return ans;
}
int main() {
#ifdef ONPC
  freopen("a.in", "r", stdin);
#endif
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout << (long long) smart(3, 5, 16) << ' ' << (long long) naive(3, 5, 16) << endl;
  return 0;
  get_mu[1] = 1;
  for (int i = 2; i < N; i++) prime[i] = true, get_mu[i] = 1;
  for (int i = 2; i < N; i++) {
    if (prime[i]) {
      for (int j = i; j < N; j += i) {
        if (j > i) {
          prime[j] = false;
        }
        get_mu[j] *= -1;
        if (j % (i * (ll) i) == 0) {
          get_mu[j] = 0;
        }
      }
    }
  }
  for (int i = 1; i < N; i++) {
    sum[i] = sum[i - 1] + get_mu[i];
  }
  int t;
  cin >> t;
  while (t--) {
    long long n, a, b, c, d;
    cin >> n >> a >> b >> c >> d;
    ll result = (solve_atmost_coprime(n, c, d)) % mod;
    result -=  (solve_less_coprime(n, a, b)) % mod;
    if (result < 0) result += mod;
    cout << (long long) (result) << '\n';
  }
  /*
  int inf = 1e9;
  ll a = rnd() % 2000 + 1, b = rnd() % 2000 + 1, c = rnd() % inf;
  cout << a << ' ' << b << ' ' << c << endl;
  cout << smart(a, b, c) << endl;
  return 0;
  int n = 1000;
  int ts = 0;
  while (true) {
    ll a = rnd() % n;
    ll b = rnd() % n;
    if (!a || !b) continue;
    ll c = rnd() % n;
    assert(smart(a, b, c) == naive(a, b, c));
    ts++;
    cout << "OK" << ' ' << ts << endl;
  }
  return 0;
  for (ll a = 1; a <= 10; a++) {
    for (ll b = 1; b <= 10; b++) {
      for (ll c = 1; c <= 10; c++) {
        cout << naive(a, b, c) << ' ' << smart(a, b, c) << endl;
      }
    }
  }
  */
}
