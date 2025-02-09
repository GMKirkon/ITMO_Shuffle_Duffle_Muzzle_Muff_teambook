/**
 * Author: Emil Lenngren, Simon Lindholm
 * Date: 2011-11-29
 * License: CC0
 * Source: folklore
 * Description: Calculates a valid assignment to boolean variables a, b, c,... to a 2-SAT problem, so that an expression of the type $(a\|\|b)\&\&(!a\|\|c)\&\&(d\|\|!b)\&\&...$ becomes true, or reports that it is unsatisfiable.
 * Negated variables are represented by bit-inversions (\texttt{\tilde{}x}).
 * Usage:
 *  TwoSat ts(number of boolean variables);
 *  ts.either(0, \tilde3); // Var 0 is true or var 3 is false
 *  ts.setValue(2); // Var 2 is true
 *  ts.atMostOne({0,\tilde1,2}); // <= 1 of vars 0, \tilde1 and 2 are true
 *  ts.solve(); // Returns true iff it is solvable
 *  ts.values[0..N-1] holds the assigned values to the vars
 * Time: O(N+E), where N is the number of boolean variables, and E is the number of clauses.
 * Status: stress-tested
 */
#pragma once
 
namespace kirkon {
struct twosat {
  digraph<int> g;
  int n;
 
  twosat(int _n) : g(digraph<int>(_n << 1)), n(_n) {}
 
  inline void add(int x, int value_x) {
    // (v[x] == value_x)
    assert(0 <= x && x < n);
    assert(0 <= value_x && value_x <= 1);
    g.add((x << 1) + (value_x ^ 1), (x << 1) + value_x);
  }
 
  inline void add(int x, int value_x, int y, int value_y) {
    // (v[x] == value_x || v[y] == value_y)
    assert(0 <= x && x < n && 0 <= y && y < n);
    assert(0 <= value_x && value_x <= 1 && 0 <= value_y && value_y <= 1);
    g.add((x << 1) + (value_x ^ 1), (y << 1) + value_y);
    g.add((y << 1) + (value_y ^ 1), (x << 1) + value_x);
  }
 
  inline void add_impl(int x, int value_x, int y, int value_y) {
    // (v[x] == value_x -> v[y] == value_y)
    assert(0 <= x && x < n && 0 <= y && y < n);
    assert(0 <= value_x && value_x <= 1 && 0 <= value_y && value_y <= 1);
    g.add((x << 1) + (value_x ^ 1), (y << 1) + (value_y ^ 1));
    //have to add reverse edge in order for algorithm prove to work :(
    g.add((y << 1) + value_y, (x << 1) + value_x);
  }
 
  inline void add_xor(int x, int y, int value) {
    // (v[x] == value_x -> v[y] == value_y)
    assert(0 <= x && x < n && 0 <= y && y < n);
    assert(0 <= value && value <= 1);
    if (value) {
			add(x, 1, y, 1);
			add(x, 0, y, 0);
    } else {
			add_impl(x, 1, y, 1);
			add_impl(x, 0, y, 0);
    }
  }
 
  inline vector<int> solve() {
    int cnt;
    vector<int> c = find_scc(g, cnt);
    vector<int> res(n);
    for (int i = 0; i < n; i++) {
      if (c[i << 1] == c[i << 1 ^ 1]) {
        return vector<int>();
      }
      res[i] = (c[i << 1] < c[i << 1 ^ 1]);
    }
    return res;
  }
};
} // namespace kirkon
