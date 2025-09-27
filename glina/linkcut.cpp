#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>

//MY OWN LINKCUT IMPLEMENTATION USING SPLAY TREES. EASY TO CALCULATE FUNCTION VALUES FOR PATHS, PERFORM LAZY PROPAGATION
//SOLUTION IS FOR PROBLEM G FROM RED PANDA CONTEST MOSCOW WORKSHOPS 2019 

using namespace std;

typedef long long ll;

mt19937 rnd(239);

struct splay_tree {
  splay_tree *l, *r, *par, *path_parent;
  int key;
  int sz, mx, mn;
  int add;
  int rev;
  int sum;
  splay_tree(int key): l(0), r(0), par(0), path_parent(0), key(key), sz(1), mx(key), mn(key), add(0), rev(0), sum(key) {}
  splay_tree() {}
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

void link(splay_tree *u, splay_tree *v) {
  recalc_push(u);
  make_root(u);
  u->path_parent = v;
}

void cut(splay_tree *u, splay_tree *v) {
  recalc_push(u);
  u = expose(u);
  u->l->par = 0;
  u->l->path_parent = 0;
  u->l = 0;
  recalc_top(u);
}

int find_ans(splay_tree *v, int r) {
  v = expose(v);
  if (v->r) {
    v->r->path_parent = v;
    v->r->par = 0;
    v->r = 0;
    recalc_top(v);
  }
  int last = 0;
  splay_tree *go = 0;
  function<void(splay_tree *)> f = [&] (splay_tree *v) {
    if (!v) return;
    if (v->mn > r) return;
    if (v->mx <= last) return;
    f(v->l);
    if (v->key <= r) {
      last = max(last, v->key);
      go = v;
    }
    f(v->r);
  };
  while (v->par) v = v->par;
  f(v);
  if (go) go = splay(go);
  return last;
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

int main() {
#ifdef ONPC
  freopen("a.in", "r", stdin);
#endif
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n, q;
  cin >> n >> q;
  vector <int> a(n);
  vector <int> go(n, n);
  vector <set <int> > arr(n);
  vector <splay_tree*> t(n);
  for (int i =0 ; i < n; i++) {
    t[i] = new splay_tree(i);
  }
  int my_pos = -1;
  auto find_nxt = [&] (int i) {
    auto it = arr[a[i]].upper_bound(i);
    int jmp = n;
    if (it != arr[a[i]].end()) {
      jmp = min(jmp, *it);
    }
    if (a[i] + 1 < n) {
      it = arr[a[i] + 1].upper_bound(i);
      if (it != arr[a[i] + 1].end()) {
        jmp = min(jmp, *it);
      }
    }
    if (my_pos != -1 && i != my_pos && jmp != my_pos && go[i] != my_pos) return;
    if (go[i] != n) {
      cut(t[i], t[go[i]]);
    }
    if (jmp != n) {
      link(t[i], t[jmp]);
    }
    go[i] = jmp;
  };
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    arr[a[i]].insert(i);
  }
  for (int i = 0; i < n; i++) {
    find_nxt(i);
  }
  for (int i = 0; i < q; i++) {
    int z;
    cin >> z;
    if (z == 1) {
      int x, y;
      cin >> x >> y;
      my_pos = x;
      int old_val = a[x];
      arr[a[x]].erase(x);
      a[x] = y;
      arr[a[x]].insert(x);
      find_nxt(x);
      auto add_prev = [&] (int val) {
        if (val < 0) return;
        auto it = arr[val].lower_bound(x);
        if (it != arr[val].begin()) {
          it--;
          find_nxt(*it);
        }
      };
      add_prev(old_val - 1);
      add_prev(old_val);
      add_prev(a[x] - 1);
      add_prev(a[x]);
    } else {
      int l, r, k;
      cin >> l >> r >> k;
      r--;
      auto it = arr[k].lower_bound(l);
      int pos = *it;
      int x = find_ans(t[pos], r);
      /*
      for (int i = 0; i < n; i++) {
        cout << go[i] << ' ';
      }
      cout << '\n';
      */
      //cout <<pos << "->" << x << endl;
      cout << a[x] << '\n';
    }
  }
}
