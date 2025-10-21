#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

mt19937 rnd(239);

const int N = 3e5 + 20;

int p[N], x[N], y[N];
int len[N];
int orz[N];
int go[N];
int vis[N];
int dsu[N];
int dsu_len[N];
int cur[N];
vector <int> g[N];
int cnt[N];
int marked[N];
int ret[N];

int zs = 2;

int get(int v) {
  if (ret[v] != zs) {
    ret[v] = zs;
    dsu[v] = v;
    dsu_len[v] = 0;
  }
  if (v == dsu[v]) return v;
  else {
    int x = get(dsu[v]);
    if (dsu_len[v] == 0) dsu_len[v]++;
    dsu_len[v] += dsu_len[dsu[v]];
    dsu[v] = x;
    return x;
  }
}

int tt = 12345;

vector <int> smart(int n, int m, vector <int> p, vector <int> x, vector <int> y) {
  for (int i = 0; i < n; i++) cur[i] =i;
  int bl_sz = 800;
  vector <int> ans;
  tt++;
  int zt = 0;
  vector <int> vs(n);
  vector <int> f(n);
  int prt = 0;
  for (int i = 0; i < m; i += bl_sz) {
    for (int i = 0; i < n; i++) {
      f[i] = get(i);
    }
    zs++;
    int r = min(i + bl_sz, m);
    for (int j = i; j < r; j++) {
      marked[x[j]] = zs;
      vs[f[x[j]]] = zs;
      cnt[x[j]]++;
    }
    zs--;

    zs++;
    for (int i= 0; i < n; i++) {
      if (vs[f[i]] != zs) {
        ret[i] = zs;
      }
    }
    /*
    for (int i = 0; i < n; i++) {
      if (cur[i] == i) marked[i] = true;
    }
    for (int i = 0; i < n; i++) {
      if (!marked[i]) {
        g[p[i]].push_back(i);
      }
    }
    queue <int> q;
    for (int i = 0; i < n; i++) {
      if (marked[i]) {
        q.push(i);
        go[i] = i;
        len[i] = 0;
      }
    }
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int x : g[v]) {
        if (!marked[x]) {
          marked[x] = true;
          go[x] = go[v];
          len[x] = len[v] + 1;
          q.push(x);
        }
      }
    }
    */
    auto f = [&] (int v) {
      tt++;
      int ans = 0;
      while (v != -1 && vis[v] != tt) {
        get(v);
        ans += dsu_len[v];
        v = get(v);
        if (vis[v] == tt) break;
        vis[v] = tt;
        if (marked[v] != zs) {
          if (get(v) == get(cur[v])) {
            ans++;
            break;
          } else {
            if (dsu[v] == v)
              dsu[v] = cur[v];
            int x = get(v);
            ans += dsu_len[v];
            v = x;
          }
        } else {
          v = cur[v];
          ans++;
        }
      }
      if (cur[v] == v) {
        return make_pair(v, ans);
      } else {
        return make_pair(-1, -1);
      }
    };
    for (int j = i; j < r; j++) {
      auto ret = f(p[j]);
      cur[x[j]] = y[j];
      cnt[x[j]]--;
      if (cnt[x[j]] == 0) marked[x[j]] = zs - 1;
      if (ret.first == -1) {
        ans.push_back(-1);
      cout << -1 << '\n';
      } else {
        ans.push_back(ret.first);
        ans.push_back(ret.second);
      cout << ret.first + 1 << ' ' << ret.second << '\n';
      }
    }
  }
  return ans;
}

vector <int> naive(int n, int m, vector <int> p, vector <int> x, vector <int> y) {
  for (int i = 0; i < n; i++) cur[i] = i;
  int bl_sz = 500;
  vector <int> ans;
  for (int i = 0; i < m; i += bl_sz) {
    int r = min(i + bl_sz, m);
    for (int i = 0; i < n; i++) marked[i] = false, go[i] = -1, dsu[i] = i, dsu_len[i] = 0;
    for (int j = i; j < r; j++) {
      marked[p[j]] = marked[x[j]] = marked[y[j]] = true;
    }
    /*
    for (int i = 0; i < n; i++) {
      if (cur[i] == i) marked[i] = true;
    }
    for (int i = 0; i < n; i++) {
      if (!marked[i]) {
        g[p[i]].push_back(i);
      }
    }
    queue <int> q;
    for (int i = 0; i < n; i++) {
      if (marked[i]) {
        q.push(i);
        go[i] = i;
        len[i] = 0;
      }
    }
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int x : g[v]) {
        if (!marked[x]) {
          marked[x] = true;
          go[x] = go[v];
          len[x] = len[v] + 1;
          q.push(x);
        }
      }
    }
    */
    auto f = [&] (int v) {
      tt++;
      int ans = 0;
      while (v != -1 && vis[v] != tt) {
        ans += dsu_len[v];
        v = get(v);
        vis[v] = tt;
        if (false) {
          if (!marked[cur[v]]) {
            if (get(v) == get(cur[v])) {
              break;
            } else {
              if (dsu[v] == v)
                dsu[v] = cur[v];
              int x = get(dsu[v]);
              ans += dsu_len[v];
              v = x;
            }
          } else {
            v = cur[v];
            ans += len[v];
          }
        } else {
          v = cur[v];
          ans++;
        }
      }
      if (cur[v] == v) {
        return make_pair(v, ans);
      } else {
        return make_pair(-1, -1);
      }
    };
    for (int j = i; j < r; j++) {
      auto ret = f(p[j]);
      cur[x[j]] = y[j];
      if (ret.first == -1) {
        ans.push_back(-1);
      } else {
        ans.push_back(ret.first);
        ans.push_back(ret.second);
      }
    }
  }
  return ans;
}


struct splay_tree {
  splay_tree *l, *r, *par, *path_parent;
  int key;
  int sz, mx, mn;
  int add;
  int rev;
  int sum;
  pair <int, int> add_edge;
  splay_tree(int key, pair <int, int> add_edge): l(0), r(0), par(0), path_parent(0), key(key), sz(1), mx(key), mn(key), add(0), rev(0), sum(key), add_edge(add_edge) {}
  splay_tree() {}
  splay_tree(int key): l(0), r(0), par(0), path_parent(0), key(key), sz(1), mx(key), mn(key), add(0), rev(0), sum(key), add_edge(make_pair(-1, -1)) {}
};

void ch(splay_tree *node, int x, int me_rv) {
  if (!node) return;
  if (me_rv) {
    swap(node->l, node->r);
    node->rev ^= 1;
  }
  node->add += x;
  node->key += x;
  node->mx += x;
  node->mn += x;
}

void push(splay_tree *node) {
  if (!node) return;
  if (!node->add && !node->rev) return;
  if (node->l) {
    ch(node->l, node->add, node->rev);
  }
  if (node->r) {
    ch(node->r, node->add, node->rev);
  }
  node->rev = 0;
  node->add = 0;
}

void recalc(splay_tree *node) {
  if (!node) return;
  node->sz = 1;
  node->mx = node->key;
  node->mn = node->key;
  node->sum = node->key;
  if (node->l) {
    node->sz += node->l->sz;
    node->mx = max(node->mx, node->l->mx);
    node->mn = min(node->mn, node->l->mn);
    node->sum += node->l->sum;
  }
  if (node->r) {
    node->sz += node->r->sz;
    node->mx = max(node->mx, node->r->mx);
    node->mn = min(node->mn, node->r->mn);
    node->sum += node->r->sum;
  }
}

void replace(splay_tree *node, splay_tree *a, splay_tree *b) {
  if (!node) return;
  if (node->l == a) {
    node->l = b;
  } else {
    node->r = b;
  }
}

splay_tree *rotate(splay_tree *node) {
  splay_tree *parpar = node->par->par;
  splay_tree *par = node->par;
  swap(node->par->path_parent, node->path_parent);
  if (node->par->l == node) {
    if (node->r) {
      node->par->l = node->r;
      node->r->par = node->par;
    } else {
      node->par->l = 0;
    }
    replace(parpar, node->par, node);
    node->par->par = node;
    node->r = node->par;
    node->par = parpar;
  } else {
    if (node->l) {
      node->par->r = node->l;
      node->l->par = node->par;
    } else {
      node->par->r = 0;
    }
    replace(parpar, node->par, node);
    node->par->par = node;
    node->l = node->par;
    node->par = parpar;
  }
  recalc(node);
  recalc(par);
  return node;
}

bool what_son(splay_tree *node) {
  if (!node) {
    assert(0);
  }
  return (node->par->r == node);
}

splay_tree *splay(splay_tree *node) {//returns root
  while (node->par != 0) {
    if (node->par->par == 0) {
      node = rotate(node);
    } else {
      if (what_son(node->par) == what_son(node)) {
        node->par = rotate(node->par);
        node = rotate(node);
      } else {
        node = rotate(node);
        node = rotate(node);
      }
    }
  }
  recalc(node);
  return node;
}

pair <splay_tree *, splay_tree *> ins(splay_tree *t, int x) {
  if (!t) {
    auto go = new splay_tree(x);
    return {go, go};
  } else {
    push(t);
    if (x < t->key) {
      auto a = ins(t->l, x);
      t->l = a.first;
      a.first->par = t;
      recalc(t);
      return {t, a.second};
    } else {
      auto a = ins(t->r, x);
      t->r = a.first;
      a.first->par = t;
      recalc(t);
      return {t, a.second};
    }
  }
}

splay_tree *insert_value(splay_tree *t, int x) {
  auto a = ins(t, x);
  t = splay(a.second);
  return t;
}

vector <int> arr;

void dfs(splay_tree *t) {
  if (!t) return;
  push(t);
  if (t->l)
    assert(t->l->par == t);
  dfs(t->l);
 // recalc(t);
  arr.push_back(t->key);
  //cout << t->key << ' ';
  if (t->r)
    assert(t->r->par == t);
  dfs(t->r);
}

pair <splay_tree*, int> get_kth(splay_tree *t, int n) {
  while (true) {
    push(t);
    if (t->l && t->l->sz >= n) {
      t = t->l;
    } else {
      if (t->l) n -= t->l->sz;
      if (n == 1) {
        break;
      } else {
        n -= 1;
        t = t->r;
      }
    }
  }
  int key = t->key;
  return {splay(t), key};
}

pair <splay_tree*, splay_tree*> split(splay_tree *t, int x) {
  if (!t) return {0, 0};
  if (t->mx <= x) return {t, 0};
  while (true) {
    push(t);
    if (t->l && t->l->mx > x) {
      t = t->l;
    } else if (t->key > x) {
      break;
    } else {
      t = t->r;
    }
  }
  t = splay(t);
  splay_tree *vl = t->l;
  if (vl) {
    vl->par = 0;
  }
  t->l = 0;
  recalc(t);
  return {vl, t};
}

splay_tree *merge(splay_tree *a, splay_tree *b) {
  if (!a) return b;
  if (!b) return a;
  push(a), push(b);
  if (a->mx > b->mx) swap(a, b);
  if (a->mx <= b->mn) {
    /*
    vector <int> ul, ur;
    arr.clear();
    dfs(a);
    ul = arr;
    arr.clear();
    dfs(b);
    ur = arr;
    for (int i = 0; i < (int) ul.size(); i++) {
      for (int j = 0; j < (int) ur.size(); j++) {
        assert(ul[i] <= ur[j]);
      }
    }
    */
    while (a->r) {
      push(a);
      a = a->r;
    }
    push(a);
    a = splay(a);
    a->r = b;
    b->par = a;
    recalc(a);
    /*
    arr.clear();
    dfs(a);
    for (int i = 1; i < (int) arr.size(); i++) {
      assert(arr[i] >= arr[i - 1]);
    }
    */
    return a;
  } else {
    /*
    if (a->sz < b->sz) swap(a, b);
    arr.clear();
    dfs(b);
    for (int x : arr) {
      a = insert_value(a, x);
    }
    return a;
    */
    int x = b->key;
    //dfs(a);
    //cout << ": ";
    auto ret = split(a, x);
    //dfs(ret.first);
    //cout << ',';
    //dfs(ret.second);
    //cout << '\n';
    if (b->l)
      b->l->par = 0;
    if (b->r)
      b->r->par = 0;
    b->l = merge(b->l, ret.first);
    b->r = merge(b->r, ret.second);
    if (b->l) {
      b->l->par = b;
    }
    if (b->r) {
      b->r->par = b;
    }
    recalc(b);
    return b;
  }
}

void recalc_push(splay_tree *v) {
  if (!v) return;
  if (v->par) {
    recalc_push(v->par);
  } else {
    recalc_push(v->path_parent);
  }
  push(v);
}

void recalc_top(splay_tree *v) {
  if (!v) return;
  recalc(v);
  recalc_top(v->par);
}

splay_tree *expose(splay_tree *v) {
  recalc_push(v);
  v = splay(v);
  while (v->path_parent) {
    push(v);
    splay_tree *p = v->path_parent;
    p = splay(p);
    if (p->r) {
      swap(p->r->par, p->r->path_parent);
    }
    push(p);
    p->r = v;
    v->par = p;
    v->path_parent = 0;
    recalc(p);
    //recalc_top(v);
    v = splay(v);
  }
  return v;
}

void make_root(splay_tree *v) {
  v = expose(v);
  if (v->r) {
    v->r->path_parent = v;
    v->r->par = 0;
    v->r = 0;
    recalc_top(v);
  }
  ch(v, 0, 1);
}


splay_tree* get_leftmost(splay_tree *t) {
  if (t->l) return get_leftmost(t->l);
  return t;
}


int get_root(splay_tree *t) {
  expose(t);
  assert(t);
  /*
  while (true) {
    if (t->path_parent) 
      t = t->path_parent;
    else
      break;
  }
  */
  auto go = get_leftmost(t);
  splay(go);
  return go->key;
}
void link(splay_tree *u, splay_tree *v) {
  recalc_push(u);
  //cout << "(" << get_root(u) << ' ' << u->key << ")" << endl;
  make_root(u);
  u->path_parent = v;
}

void cut(splay_tree *u, splay_tree *v) {
  recalc_push(u);
  u = expose(u);
  u->l->par = 0;
  u->l = 0;
  recalc_top(u);
}

int sum(splay_tree *u, splay_tree *v) {
  make_root(u);
  expose(v);
  if (v->r) {
    v->r->path_parent = v;
    v->r->par = 0;
    v->r = 0;
    recalc_top(v);
  }
  return v->sum;
}


splay_tree *rt[N];

int get_len(splay_tree *t) {
  t = expose(t);
  int sum = t->sz;
  if (t->r) sum -= t->r->sz;
  return sum;
}

int main() {
#ifdef ONPC
  freopen("a.in", "r", stdin);
  //freopen("smart.out", "w", stdout);
#endif
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n = 3e5, m = 3e5;
  for (int i = 0; i < n; i++) {
    rt[i] = new splay_tree(i, make_pair(i, i));
  }
  vector <int> cur(n);
  for (int i = 0; i < n; i++) cur[i] = i;
  auto add_edge = [&] (int from, int to) {
    int u = get_root(rt[from]);
    int v = get_root(rt[to]);
    if (u != v) {
      link(rt[from], rt[to]);
    } else {
      rt[u]->add_edge = make_pair(from, to);
    }
  };
  auto del_edge = [&] (int from, int to) {
    int u = get_root(rt[from]);
    auto val = rt[u]->add_edge;
    //cout << rt[u]->add_edge.first << ' ' << rt[u]->add_edge.second << endl;
    if (rt[u]->add_edge == make_pair(from, to)) {
      rt[u]->add_edge = make_pair(-1, -1);
    } else {
      cut(rt[from], rt[to]);
      if (val.first != -1 && get_root(rt[val.first]) != get_root(rt[val.second])) {
        link(rt[val.first], rt[val.second]); 
        rt[u]->add_edge = make_pair(-1, -1);
      }
    }
  };
  cin >> n >> m;
  //n = 10, m = 10;
  //n = 10, m = 10;
  vector <int> p(m), x(m) ,y(m);
  vector <int> ans;
  for (int i = 0; i < m; i++) {
    p[i] = rnd() % n, x[i] = rnd() % n, y[i] = rnd() % n;
    cin >> p[i] >> x[i] >> y[i];
    p[i]--, x[i]--, y[i]--;
    int t = get_root(rt[p[i]]);
     if (cur[t] == t) {
       cout << t + 1 << ' ' << get_len(rt[p[i]]) << '\n';
       ans.push_back(t);
       ans.push_back(get_len(rt[p[i]]));
    } else {
      cout << -1 << '\n';
        ans.push_back(-1);
    }
    del_edge(x[i], cur[x[i]]);
    cur[x[i]] = y[i];
    add_edge(x[i], cur[x[i]]);
  }
  //auto go = smart(n, m, p, x, y);
  /*
  for (int x : go) cout << x << ' ';
  cout << endl;
  for (int x : ans) cout << x << ' ';
  cout << endl;
  cout << "OK" << endl;
  */
}
