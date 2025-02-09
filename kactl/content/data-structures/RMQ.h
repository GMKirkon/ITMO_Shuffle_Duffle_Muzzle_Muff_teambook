/**
 * Author: Johan Sannemo, pajenegod
 * Date: 2015-02-06
 * License: CC0
 * Source: Folklore
 * Description: Range Minimum Queries on an array. Returns
 * min(V[a], V[a + 1], ... V[b - 1]) in constant time.
 * Usage:
 *  RMQ rmq(values);
 *  rmq.query(inclusive, exclusive);
 * Time: $O(|V| \log |V| + Q)$
 * Status: stress-tested
 */
// usage:
//   auto fun = [&](int i, int j) { return min(i, j); };
//   SparseTable<int, decltype(fun)> st(a, fun);
// or:
//   SparseTable<int> st(a, [&](int i, int j) { return min(i, j); });
template <typename T, class F = function<T(const T&, const T&)>>
class SparseTable {
 public:
  int n;
  vector<vector<T>> mat;
  F func;

  SparseTable(const vector<T>& a, const F& f) : func(f) {
    n = static_cast<int>(a.size());
    int max_log = 32 - __builtin_clz(n);
    mat.resize(max_log);
    mat[0] = a;
    for (int j = 1; j < max_log; j++) {
      mat[j].resize(n - (1 << j) + 1);
      for (int i = 0; i <= n - (1 << j); i++) {
        mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
      }
    }
  }

  T get(int from, int to) const {
    assert(0 <= from && from <= to && to <= n - 1);
    int lg = 32 - __builtin_clz(to - from + 1) - 1;
    return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
  }
};

template <typename T, typename Func>
class DisjointSparseTable {
 public:
  int _n;
  vector<vector<T>> _matrix;
  Func _func;

  DisjointSparseTable(const vector<T>& a, const Func& func) : _n(static_cast<int>(a.size())), _func(func) {
    _matrix.push_back(a);
    for (int layer = 1; (1 << layer) < _n; ++layer) {
      _matrix.emplace_back(_n);
      for (int mid = 1 << layer; mid < _n; mid += 1 << (layer + 1)) {
        _matrix[layer][mid - 1] = a[mid - 1];
        for (int j = mid - 2; j >= mid - (1 << layer); --j) {
          _matrix[layer][j] = _func(a[j], _matrix[layer][j + 1]);
        }
        _matrix[layer][mid] = a[mid];
        for (int j = mid + 1; j < min(_n, mid + (1 << layer)); ++j) {
          _matrix[layer][j] = _func(_matrix[layer][j - 1], a[j]);
        }
      }
    }
  }

  T Query(int l, int r) const {
    assert(0 <= l && l < r && r <= _n);
    if (r - l == 1) {
      return _matrix[0][l];
    }
    int layer = 31 - __builtin_clz(l ^ (r - 1));
    return _func(_matrix[layer][l], _matrix[layer][r - 1]);
  }
};
