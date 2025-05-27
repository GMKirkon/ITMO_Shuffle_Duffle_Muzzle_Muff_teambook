/**
 * Author: Kiril Konovalov
 * Date: 2025-03-24
 * License: CC0
 * Source: https://github.com/the-tourist/algo/blob/master/graph/twosat.cpp although heavily modified from tourist's code
 * Description: Calculates a valid assignment to boolean variables a, b, c,... to a 2-SAT problem, so that an expression of the type $(a\|\|b)\&\&(!a\|\|c)\&\&(d\|\|!b)\&\&...$ becomes true, or reports that it is unsatisfiable.
 * Status: stress-tested
 */
#pragma once
#include "../../stress-tests/utilities/template.h"
#include "graphs_structures.h"
#include "SCC.h"
 
struct twosat {
  digraph<int> g; int n;
  twosat(int _n) : g(digraph<int>(_n << 1)), n(_n) {}
  inline void add(int x, int value_x) { // (v[x] == value_x)
    assert(0 <= x && x < n); assert(0 <= value_x && value_x <= 1);
    g.add((x << 1) + (value_x ^ 1), (x << 1) + value_x); }
  inline void add(int x, int value_x, int y, int value_y) { // (v[x] == value_x || v[y] == value_y)
    assert(0 <= x && x < n && 0 <= y && y < n); assert(0 <= value_x && value_x <= 1 && 0 <= value_y && value_y <= 1);
    g.add((x << 1) + (value_x ^ 1), (y << 1) + value_y); g.add((y << 1) + (value_y ^ 1), (x << 1) + value_x); }
  inline void add_impl(int x, int value_x, int y, int value_y) { // (v[x] == value_x -> v[y] == value_y)
    assert(0 <= x && x < n && 0 <= y && y < n); assert(0 <= value_x && value_x <= 1 && 0 <= value_y && value_y <= 1);
    g.add((x << 1) + (value_x ^ 1), (y << 1) + (value_y ^ 1)); g.add((y << 1) + value_y, (x << 1) + value_x); }
  inline void add_xor(int x, int y, int value) { // (v[x] == value_x -> v[y] == value_y) 
    assert(0 <= x && x < n && 0 <= y && y < n); assert(0 <= value && value <= 1);
    if (value) { add(x, 1, y, 1); add(x, 0, y, 0); }
    else { add_impl(x, 1, y, 1); add_impl(x, 0, y, 0); } }
  vi solve() { int cnt; vi c = find_scc(g, cnt); vi res(n);
    for (int i = 0; i < n; i++) {
      if (c[i << 1] == c[i << 1 ^ 1]) return vi();
      res[i] = (c[i << 1] < c[i << 1 ^ 1]);
    }
    return res; }
};