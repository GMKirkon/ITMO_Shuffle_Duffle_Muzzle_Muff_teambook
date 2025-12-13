#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

mt19937 rnd(239);

struct pt {
  ll x, y;
  pt(ll x, ll y): x(x), y(y) {}
  pt() {}
};

pt operator - (pt a, pt b) {
  return {a.x - b.x, a.y - b.y};
}

pt operator + (pt a, pt b) {
  return {a.x + b.x, a.y + b.y};
}

__int128 operator ^ (pt a, pt b) {
  return a.x * (__int128) b.y - a.y * (__int128) b.x;
}

pt s;

int part(pt p) {
  if (p.x <= 0) {
    if (p.y <= 0) return 1;
    return 2;
  } else {
    if (p.y <= 0) return 4;
    return 3;
  }
}

bool operator < (pt a, pt b) {
  a = a - s;
  b = b - s;
  return (a ^ b) > 0;
}

vector <pt> hull(vector <pt> a) {
  sort(a.begin(), a.end(), [&] (pt a, pt b) {
    return make_pair(a.x, a.y) < make_pair(b.x, b.y);
  });
  s = a[0];
  sort(a.begin() + 1, a.end());
  vector <pt> h;
  auto bad = [&] (pt a, pt b, pt c) {
    return ((b - a) ^ (c - b)) <= 0;
  };
  for (int i = 0; i < (int) a.size(); i++) {
    while (h.size() > 1 && bad(h[(int) h.size() - 2], h.back(), a[i])) {
      h.pop_back();
    }
    h.push_back(a[i]);
  }
  return h;
}


void reorder_polygon(vector<pt> & P){
    size_t pos = 0;
    for(size_t i = 1; i < P.size(); i++){
        if(P[i].y < P[pos].y || (P[i].y == P[pos].y && P[i].x < P[pos].x))
            pos = i;
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

vector<pt> minkowski(vector<pt> P, vector<pt> Q){
    // the first vertex must be the lowest
    reorder_polygon(P);
    reorder_polygon(Q);
    // we must ensure cyclic indexing
    P.push_back(P[0]);
    P.push_back(P[1]);
    Q.push_back(Q[0]);
    Q.push_back(Q[1]);
    // main part
    vector<pt> result;
    size_t i = 0, j = 0;
    while(i < P.size() - 2 || j < Q.size() - 2){
        result.push_back(P[i] + Q[j]);
        auto cross = (P[i + 1] - P[i]) ^ (Q[j + 1] - Q[j]);
        if(cross >= 0 && i < P.size() - 2)
            ++i;
        if(cross <= 0 && j < Q.size() - 2)
            ++j;
    }
    return result;
}

ll ans = 0;

ll f(pt p) {
  return p.x * (ll) p.x + p.y * (ll) p.y;
}

void solve(vector <pt> a, vector <pt> b) {
  if (a.empty() || b.empty()) return;
  a = hull(a);
  for (auto &c : b) {
    c.x *= -1, c.y *= -1;
  }
  b = hull(b);
  a = minkowski(a, b);
  for (auto z : a) {
    ans = max(ans, z.x * (ll) z.x + z.y * (ll) z.y);
  }
}

int main() {
#ifdef ONPC
  freopen("a.in", "r", stdin);
#endif
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n;
  cin >> n;
  vector <pt> p(n);
  vector <int> t(n);
  set <pair <int, int> > qq;
  bool good = true;
  for (int i = 0; i < n; i++) {
    cin >> p[i].x >> p[i].y >> t[i];
    t[i]--;
  }
  for (int k = 0; (1 << k) < n; k++) {
    vector <pt> a, b;
    for (int i = 0; i < n; i++) {
      if ((t[i] >> k) & 1) {
        a.push_back(p[i]);
      } else {
        b.push_back(p[i]);
      }
    }
    solve(a, b);
    solve(b, a);
  }
  ll f = 0;
  cout << ans << endl;
}

