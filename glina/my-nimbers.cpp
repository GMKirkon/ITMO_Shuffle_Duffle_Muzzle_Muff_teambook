#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ll;
typedef long double ld;

mt19937 rnd(239);

ll mul(ll a, ll b);
ll mul(int x, vector <int> a);

ll mul(vector <int> arr) {
  if (arr.empty()) return 1;
  {
    vector <int> cur = arr;
    cur.pop_back();
    ll go = mul(cur);
    if (go < ((ll) (1) << (1 << arr.back()))) {
      return go * ((ll) (1) << (1 << arr.back()));
    } 
    if (rnd() % 5 == 0)
      return mul(go, ((ll) (1) << (1 << arr.back())));
  }
  {
    int pos = rnd() % (int) arr.size();
    if (rnd() % 3 == 0) {
      ll v = ((ll) (1) << ((ll) (1) << arr[pos]));
      auto cur = arr;
      cur.erase(cur.begin() + pos);
      return mul(mul(cur), v);
    }
  }
  while (arr.size() > 1) {
    if (arr[0] != arr[1]) {
      ll value = (ll) (((ll) (1) << arr[0])) + (ll) (((ll) (1) << arr[1]));
      arr.erase(arr.begin());
      arr.erase(arr.begin());
      return mul(value, arr);
    } else {
      ll a = ((ll) (1) << arr[0]) - 1;
      ll b = ((ll) (1) << arr[0]);
      arr.erase(arr.begin());
      arr.erase(arr.begin());
      return mul(a, arr) ^ mul(b, arr);
    }
  }
  return ((ll) (1) << ((ll) (1) << arr[0]));
}

ll mul(int p, vector <int> a) {
  if (a.empty()) return ((ll) (1) << p);
  for (int i = 5; i >= 0; i--) {
    if ((p >> i) & 1) {
      a.push_back(i);
    }
  }
  sort(a.begin(), a.end());
  return mul(a);
}

ll pw_mul(int i, int j) {
  vector <int> arr;
  for (int a = 0; a < 6; a++) {
    if ((i >> a) & 1) {
      arr.push_back(a);
    }
    if ((j >> a) & 1) {
      arr.push_back(a);
    }
  }
  ll ans = mul(arr);
  return ans;
}

ll mul(ll a, ll b) {
  ll ans = 0;
  for (int i = 63; i >= 0; i--) {
    for (int j = 63; j >= 0; j--) {
      if ((a >> i) & 1) {
        if ((b >> j) & 1) {
          ans ^= pw_mul(i, j);
        }
      }
    }
  }
  return ans;
}

const int len = 15;
const int m = 64 / len + 1;

ll p[64][64];
ll p_prec[64][m][1 << len];


int main() {
#ifdef ONPC
  freopen("a.in", "r", stdin);
#endif
  ios::sync_with_stdio(0);
  cin.tie(0);
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 64; j++) {
      for (auto c : arr[i * 64 + j]) {
        p[i][j] = p[i][j] * 10 + c - '0';
      }
    }
  }
  auto my_mul = [&] (ll a, ll b) {
    ll s = 0;
    for (int i = 63; i >= 0; i--) {
      if ((a >> i) & 1) {
        for (int j = 63; j >= 0; j--) {
          if ((b >> j) & 1) {
            s ^= p[i][j];
          }
        }
      }
    }
    return s;
  };
  for (int i = 0; i < 64; i++) {
    for (int bl = 0; bl * len < 64; bl++) {
      int l = bl * len, r = bl * len + len - 1;
      r = min(r, 63);
      for (int mask = 0; mask < (1 << len); mask++) {
        for (int j = l; j <= r; j++) {
          if ((mask >> (j - l)) & 1) {
            p_prec[i][bl][mask] ^= p[i][j];
          }
        }
      }
    }
  }
  auto fast_mul = [&] (ll a, ll b) {
    ll ans = 0;
    for (int i = 0; i < 64; i++) {
      if ((a >> i) & 1) {
        ll x = b;
        for (int j = 0; j * len < 64; j++) {
          ans ^= p_prec[i][j][x & ((1 << len) - 1)];
          x >>= len;
        }
      }
    }
    return ans;
  };
  auto get_pw = [&] (ll a, ll n) {
    ll ret = 1;
    while (n) {
      if (n % 2 == 0) {
        a = fast_mul(a, a);
        n /= 2;
      } else {
        ret = fast_mul(ret, a);
        n--;
      }
    }
    return ret;
  };
  auto inv = [&] (ll x) {
    return get_pw(x, ULLONG_MAX - 1);
  };
  int n;
  cin >> n;
  vector <vector <ll> > a(n, vector <ll> (n));
  for (int i = 0;i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> a[i][j];
    }
  }
  for (int i = 0; i < n; i++) {
    int k = i;
    for (int j = i + 1; j < n; j++) {
      if (a[j][i] > a[k][i]) k = j;
    }
    if (!a[k][i]) {
      cout << "Second\n";
      return 0;
    }
    swap(a[i], a[k]);
    ll x = inv(a[i][i]);
    for (int j = i + 1; j < n; j++) {
      a[i][j] = fast_mul(a[i][j], x);
    }
    for (int j = 0; j < n; j++) {
      if (j != i && a[j][i]) {
        for (int k = i + 1; k < n; k++) {
          a[j][k] ^= fast_mul(a[i][k], a[j][i]);
        }
      }
    }
  }
  cout << "First\n";
}
