/**
 * Author: Kirkon and BenQ
 * Date: 2024, improved in 2025
 * License: CC0
 * Source: BenQ code improved
 * Description: creates hashes
 * Time: O(N)
 * Status: tested on many problems
 */

#include "../../stress-tests/utilities/template.h"

constexpr int HASH_MOD = MOD; constexpr int HASH_SIZE = 2;
uniform_int_distribution<int> BDIST(0.1 * HASH_MOD, 0.9 * HASH_MOD);
struct custom_hash {
  array<int, HASH_SIZE> vals{};
  custom_hash() { vals.fill(0); }

  custom_hash(const array<int, HASH_SIZE> &other) { vals = other; }
  custom_hash(array<int, HASH_SIZE> &&other) { vals = std::move(other); }
  custom_hash &operator=(const array<int, HASH_SIZE> &other) { vals = other; return *this; }
  custom_hash &operator=(array<int, HASH_SIZE> &&other) { vals = std::move(other); return *this; }

  int &operator[](int x) { return vals[x]; }

  // if C++ 20 is availiable use auto operator<=> and bool operator== instead
  bool operator==(const custom_hash &other) const { return vals == other.vals; }
  bool operator!=(const custom_hash &other) const { return vals != other.vals; }
  bool operator<(const custom_hash &other) const { return vals < other.vals; }
  bool operator>(const custom_hash &other) const { return vals > other.vals; }
  bool operator<=(const custom_hash &other) const { return vals <= other.vals; }
  bool operator>=(const custom_hash &other) const { return vals >= other.vals; }
};

template<class T> custom_hash make_hash(T c) { auto res = custom_hash{}; res.vals.fill(c); return res; }

custom_hash base{};
vector<custom_hash> pows{};

custom_hash operator+(custom_hash l, custom_hash r) {
  for (int i = 0; i < HASH_SIZE; ++i) if ((l[i] += r[i]) >= HASH_MOD) l[i] -= HASH_MOD; return l;
}
custom_hash operator-(custom_hash l, custom_hash r) {
  for (int i = 0; i < HASH_SIZE; ++i) if ((l[i] -= r[i]) < 0) l[i] += HASH_MOD; return l;
}
custom_hash operator*(custom_hash l, custom_hash r) { 
  for (int i = 0; i < HASH_SIZE; ++i) l[i] = (ll) l[i] * r[i] % HASH_MOD; return l;
}


void init() {
  static bool used = false; if (exchange(used, true)) { return; }
  for (auto &u: base.vals) { u = BDIST(rng); }
  pows.emplace_back(make_hash(1));
}

struct HashRange {
  str S; vector<custom_hash> cum{};
  HashRange() { init(); cum.emplace_back(); }
  void add(char c) { S += c; cum.pb(base * cum.back() + make_hash(c));}
  void add(str s) { each(c, s) add(c); }
  void extend(int len) { while (sz(pows) <= len) pows.pb(base * pows.back()); }
  custom_hash hash(int l, int r) { int len = r + 1 - l; extend(len); return cum[r + 1] - pows[len] * cum[l]; }
};

struct custom_int_hash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
};

custom_int_hash int_hash{};

namespace std {
  template<>
  struct hash<custom_hash> {
    inline size_t operator()(const custom_hash& x) const {
      size_t result = 0; for (auto u : x.vals) result ^= int_hash(u);
      return custom_int_hash::splitmix64(result);
    }
  };
}
