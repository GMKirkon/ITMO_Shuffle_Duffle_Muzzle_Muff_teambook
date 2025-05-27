/**
 * Author: Kirill Konovalov
 * Date: 2025-03
 * License: CC0
 * Description: linear CHT and persistent linear CHT for monotonic slopes
 * Time: O(\log N)
 */
#pragma once

#include "../../stress-tests/utilities/template.h"

constexpr int64_t CHT_LINE_INF = std::numeric_limits<int64_t>::max();

struct line {
  int a{0};
  int64_t b{CHT_LINE_INF};
  line() = default;
  line(int a, int64_t b) noexcept : a(a), b(b) {}
  friend __int128 cross(const line &left, const line &right) {
    return (__int128)left.a * right.b - (__int128)left.b * right.a;
  }
  friend line operator-(const line &left, const line &right) {
    return {left.a - right.a, left.b - right.b};
  }
  [[nodiscard]] int64_t evaluate(int x) const { return (int64_t)a * x + b; }
};

// query for min, inserting for increasing/decreasing slope
template <bool increasing_slope> struct incremental_CHT {
  vector<line> lines;
  void insert(const line &new_line) {
    while (lines.size() >= 2) {
      const line &line1 = lines[lines.size() - 2], line2 = lines.back(), line3 = new_line;
      if ((cross(line3 - line2, line2 - line1) < 0) ^ increasing_slope) { // > 0 for max
        lines.pop_back();
      } else break;
    }

    if (!lines.empty()) {
      const line &line1 = lines.back(), line2 = new_line;
      if (line1.a == line2.a) {
        if (line1.b > line2.b) lines.pop_back(); // < for max
        else return;
      }
    }
    lines.push_back(new_line);
  }

  // have to perform queries with decreasing x
  int64_t query(int x) {
    assert(!lines.empty());
    while (lines.size() >= 2) {
      const line &line1 = lines[lines.size() - 2], line2 = lines.back();
      if (line1.evaluate(x) < line2.evaluate(x)) lines.pop_back(); // > for max
      else break;
    }
    return lines.back().evaluate(x);
  }
};

template <bool increasing_slope> struct persistent_incremental_CHT {
  vector<line> lines;
  vector<int> parents;
  int max_query_alive_vertex{};
  persistent_incremental_CHT() = default;

  int insert(const line &new_line) {
    parents.push_back(-1); lines.push_back(new_line); max_query_alive_vertex = lines.size() - 1;
    if (lines.size() == 1) return 0;
    int k = lines.size() - 1, j = lines.size() - 2, i = parents[j];
    parents[k] = j;
    while (i != -1) {
      const line &line1 = lines[i], line2 = lines[j], line3 = new_line;
      if ((cross(line3 - line2, line2 - line1) > 0) ^ !increasing_slope) { // < 0 for max
        parents[k] = i; j = i; i = parents[i];
      } else break;
    }

    while (j != -1) {
      const line &line1 = lines[j], line2 = new_line;
      if (line1.a == line2.a) {
        if (line1.b > line2.b) parents[k] = j = parents[j]; // < for max
        else return k;
      } else break;
    }
    return k;
  }

  int get(int root, int x) {
    if (root == -1 || parents[root] == -1) return root;
    const line &line1 = lines[parents[root]], line2 = lines[root];
    if (line1.evaluate(x) < line2.evaluate(x)) return parents[root] = get(parents[root], x);
    else return root;
  }

  // have to perform queries with increasing x if slope increases
  int64_t query(int root, int x) {
    assert(!lines.empty());
    int64_t answer = lines[root].evaluate(x);
    return min(answer, lines[get(root, x)].evaluate(x));
  }
};