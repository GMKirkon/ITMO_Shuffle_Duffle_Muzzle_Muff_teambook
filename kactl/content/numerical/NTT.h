/**
 * Author: chilli
 * Date: 2019-04-16
 * License: CC0
 * Source: based on KACTL's FFT
 * Description: ntt(a) computes $\hat f(k) = \sum_x a[x] g^{xk}$ for all $k$, where $g=\text{root}^{(mod-1)/N}$.
 * N must be a power of 2.
 * Useful for convolution modulo specific nice primes of the form $2^a b+1$,
 * where the convolution result has size at most $2^a$. For arbitrary modulo, see FFTMod.
   \texttt{conv(a, b) = c}, where $c[x] = \sum a[i]b[x-i]$.
   For manual convolution: NTT the inputs, multiply
   pointwise, divide by n, reverse(start+1, end), NTT back.
 * Inputs must be in [0, mod).
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

#include "../number-theory/ModPow.h"

const ll mod = (119 << 23) + 1, root = 62; // = 998244353
// For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). The last two are > 10^9.
typedef vector<ll> vl;
void ntt(vl &a) {
	int n = sz(a), L = 31 - __builtin_clz(n);
	static vl rt(2, 1);
	for (static int k = 2, s = 2; k < n; k *= 2, s++) {
		rt.resize(n);
		ll z[] = {1, modpow(root, mod >> s)};
		rep(i,k,2*k) rt[i] = rt[i / 2] * z[i & 1] % mod;
	}
	vi rev(n);
	rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	rep(i,0,n) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
			ll z = rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
			a[i + j + k] = ai - z + (z > ai ? mod : 0);
			ai += (ai + z >= mod ? z - mod : z);
		}
}
vl conv(const vl &a, const vl &b) {
	if (a.empty() || b.empty()) return {};
	int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s), n = 1 << B;
	int inv = modpow(n, mod - 2);
	vl L(a), R(b), out(n);
	L.resize(n), R.resize(n);
	ntt(L), ntt(R);
	rep(i,0,n) out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
	ntt(out);
	return {out.begin(), out.begin() + s};
}

using Mat = vector<vl>;

//a is NxN, b is MxM
Mat conv2d_ntt(const Mat& A, const Mat& B) {
  int ha = (int)A.size(); int wa = ha ? (int)A[0].size() : 0;
  int hb = (int)B.size(); int wb = hb ? (int)B[0].size() : 0;
  if (!ha || !wa || !hb || !wb) return {};
	int H  = ha + hb - 1;
  int wr = wa + wb - 1;

  // Flatten with stride = wr so column indices never alias between rows.
  vl A1(ha * wr, 0), B1(hb * wr, 0);
  rep(i, 0, ha) rep(j, 0, wa) {
  	ll x = A[i][j] % mod;
    A1[i * wr + j] = x + (x < 0) * mod;
  }
  
  rep(i, 0, hb) rep(j, 0, wb) {
  	ll x = B[i][j] % mod;
    B1[i * wr + j] = x + (x < 0) * mod;
  }        

  vl C1 = conv(A1, B1);
  Mat R(H, vl(wr, 0));
  rep(i, 0, H) rep(j, 0, wr) R[i][j] = C1[i * wr + j] % mod;
  return R;
}

Mat conv2d_brute(Mat a, Mat b) {
  int n = a.size(), m = b.size();
  Mat ans(n + m - 1, vl(n + m - 1, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int r = 0; r < m; r++) {
        for (int c = 0; c < m; c++) {
          ans[i + r][j + c] += 1LL * a[i][j] * b[r][c] % mod;
          ans[i + r][j + c] %= mod;
        }
      }
    }
  }
  return ans;
}

Mat rot180(const Mat& K) {
    int h = (int)K.size(), w = h ? (int)K[0].size() : 0;
    Mat R(h, vl(w, 0));
    rep(i, 0, h) rep(j, 0, w)
      R[h - 1 - i][w - 1 - j] = (K[i][j] % MOD + MOD) % MOD;
    return R;
}

Mat correlate2d_ntt(const Mat& A, const Mat& P) {
    return conv2d_ntt(A, rot180(P));
}

// Crop the "valid" region (top-left placements of pattern in grid)
Mat crop_valid(const Mat& Cfull, int ha, int wa, int hb, int wb) {
    int H = ha - hb + 1, W = wa - wb + 1;
    int offi = hb - 1, offj = wb - 1;
    Mat R(H, vl(W, 0));
    rep(i, 0, H) rep(j, 0, W)
      R[i][j] = Cfull[i + offi][j + offj];
    return R;
}
