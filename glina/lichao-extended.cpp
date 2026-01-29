#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#define int long long
// #define double long double
using namespace std;

using ll = long long;

const long long inf = 1e18;

template<typename data_t>
struct Line {
  data_t a, b;
  Line() : a(0), b(inf) {}
  Line(data_t a, data_t b) : a(a), b(b) {}
  data_t get(data_t x) {
    return a * x + b;
  }
  void add(Line x) {
    a += x.a;
    b += x.b;
  }
};

class LiChao {
 private:
  using data_t = long long;

  struct Node {
    Line<data_t> line = Line<data_t>();
    Line<data_t> lazy = Line<data_t>(0, 0);

    Node *lc = nullptr;
    Node *rc = nullptr;

    void apply(data_t l, data_t r, Line<data_t> v) {
      line.add(v);
      lazy.add(v);
    }
  };

  data_t sz;
  Node *root = nullptr;

  void PushLazy(Node* &n, data_t tl, data_t tr) {
    if (n == nullptr) return;
    if (n->lc == nullptr) n->lc = new Node();
    if (n->rc == nullptr) n->rc = new Node();
    data_t mid = (tl + tr) / 2;
    n->lc->apply(tl, mid, n->lazy);
    n->rc->apply(mid + 1, tr, n->lazy);
    n->lazy = Line<data_t>(0, 0);
  }

  void PushLine(Node* &n, data_t tl, data_t tr) {
    if (n == nullptr) return;
    data_t mid = (tl + tr) / 2;
    InsertLineKnowingly(n->lc, tl, mid, n->line);
    InsertLineKnowingly(n->rc, mid + 1, tr, n->line);
    n->line = Line<data_t>();
  }

  void InsertLineKnowingly(Node* &n, data_t tl, data_t tr, Line<data_t> x) {
    if (n == nullptr) n = new Node();
    if (n->line.get(tl) > x.get(tl)) swap(n->line, x);
    if (n->line.get(tr) <= x.get(tr)) return;
    if (tl == tr) return;
    data_t mid = (tl + tr) / 2;
    PushLazy(n, tl, tr);
    if (n->line.get(mid) <= x.get(mid)) {
      InsertLineKnowingly(n->rc, mid + 1, tr, x);
    } else {
      swap(n->line, x);
      InsertLineKnowingly(n->lc, tl, mid, x);
    }
  }

  void InsertLine(Node* &n, data_t tl, data_t tr, data_t l, data_t r, Line<data_t> x) {
    if (tr < l || r < tl || tl > tr || l > r) return;
    if (n == nullptr) n = new Node();
    if (l <= tl && tr <= r) return InsertLineKnowingly(n, tl, tr, x);
    data_t mid = (tl + tr) / 2;
    PushLazy(n, tl, tr);
    InsertLine(n->lc, tl, mid, l, r, x);
    InsertLine(n->rc, mid + 1, tr, l, r, x);
  }

  void AddLine(Node* &n, data_t tl, data_t tr, data_t l, data_t r, Line<data_t> x) {
    if (tr < l || r < tl || tl > tr || l > r) return;
    if (n == nullptr) n = new Node();
    if (l <= tl && tr <= r) return n->apply(tl, tr, x);
    data_t mid = (tl + tr) / 2;
    PushLazy(n, tl, tr);
    PushLine(n, tl, tr);
    AddLine(n->lc, tl, mid, l, r, x);
    AddLine(n->rc, mid + 1, tr, l, r, x);
  }

  data_t Query(Node* &n, data_t tl, data_t tr, data_t x) {
    if (n == nullptr) return inf;
    if (tl == tr) return n->line.get(x);
    data_t res = n->line.get(x);
    data_t mid = (tl + tr) / 2;
    PushLazy(n, tl, tr);
    if (x <= mid) {
      res = min(res, Query(n->lc, tl, mid, x));
    } else {
      res = min(res, Query(n->rc, mid + 1, tr, x));
    }
    return res;
  }

 public:
  LiChao() {}
  LiChao(data_t sz) : sz(sz) {}

  void InsertLine(data_t l, data_t r, Line<data_t> x) {
    return InsertLine(root, 0, sz - 1, l, r, x);
  }
  void AddLine(data_t l, data_t r, Line<data_t> x) {
    return AddLine(root, 0, sz - 1, l, r, x);
  }
  data_t Query(data_t x) {
    return min(inf, Query(root, 0, sz - 1, x));
  }
};

void solve() {
    int x, k; cin >> x >> k;
    int n, m; cin >> n >> m;
    vector<vector<pair<int, int>>> evac(k + 1);
    for (int i = 0; i < n; ++i) {
        int p, q, r; cin >> p >> q >> r;
        evac[q].push_back({p, r});
    }
    vector<vector<array<int, 3>>> obs(k + 1);
    for (int i = 0; i < m; ++i) {
        int s, e, y, t; cin >> s >> e >> y >> t;
        obs[y].push_back({s, e, t});
    }
    vector<int> c(k);
    for (int i = 1; i < k; ++i) cin >> c[i];
    const int INF = 4e18;
    int sz = x + 2;
    LiChao lichao(x + 2);
   // vector<int> dp(sz, INF);
      for (auto [p, r] : evac[1]) {
        lichao.InsertLine(0, p, Line<ll>(-c[1],r+p*(ll)c[1]));
        lichao.InsertLine(p, sz - 1, Line<ll>(c[1],r-p*(ll)c[1]));
      }
    for (int y = 2; y < k; ++y) {
        for (auto [s, e, t] : obs[y]) {
          lichao.AddLine(s, e, Line<ll> (0ll, t));
          /*
            for (int xx = s; xx <= e; ++xx)
                new_dp[xx] += t;
                */
        }
        for (auto [s, e, t] : obs[y]) {
          evac[y].push_back({s - 1, lichao.Query(s - 1)});
          evac[y].push_back({e + 1, lichao.Query(e + 1)});
        }
        for (auto [p, r] : evac[y]) {
          lichao.InsertLine(0, p, Line<ll>(-c[y],r+p*(ll)c[y]));
          lichao.InsertLine(p, sz - 1, Line<ll>(c[y],r-p*(ll)c[y]));
        }
            //new_dp[p] = min(new_dp[p], r);

        /*
        vector<int> pref(x + 2), suf(x + 2);
        for (int z = 0; z <= x + 1; ++z) {
            pref[z] = new_dp[z] - z * c[y];
            if (z)
                pref[z] = min(pref[z], pref[z - 1]);
        }
        for (int z = x + 1; z >= 0; --z) {
            suf[z] = new_dp[z] + z * c[y];
            if (z < x + 1)
                suf[z] = min(suf[z], suf[z + 1]);
        }

        for (int xx = 0; xx <= x + 1; ++xx) {
            dp[xx] = min(pref[xx] + xx * c[y], suf[xx] - xx * c[y]);
        }
        */
    }
    for (int xx = 1; xx <= x; ++xx) {
      cout << lichao.Query(xx) << '\n';
        //cout << dp[xx] << "\n";
    }
}

int32_t main() {
#ifdef ONPC
  freopen("a.in", "r", stdin);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;// cin >> T;
    while (T--)
        solve();
    return 0;
}
