/**
 * Author: Kirill Konovalov
 * Date: 2025-05
 * License: CC0
 * Source: Effective C++ for HFT
 * Description: Basic operations on matrices. NEVER FORGET TRANSPOSING BEFORE MULTIPLYING.
 * Usage: Matrix<int, 3> A;
 *  A.d = {{{{1,2,3}}, {{4,5,6}}, {{7,8,9}}}};
 *  vector<int> vec = {1,2,3};
 *  vec = (A^N) * vec;
 * Status: tested
 */
#pragma once

template<class T, int N, int M> struct Matrix {
	using M1 = Matrix<T, M, N>; using M2 = Matrix<T, N, M>;
	M1 d{};
	M2 transpose() const {
		M2 a;
		rep(i,0,N) rep(j,0,M) a.d[j][i] = d[i][j];
		return a;
	}
	template<int K>
	Matrix<T, N, K> operator*(const Matrix<T, M, K>& b) const {
		Matrix<T, N, K> a;
		Matrix<T, K, M> b_t = b.transpose();
		rep(i,0,N) rep(j,0,K) rep(k,0,M) a.d[i][j] += d[i][k]*b_t.d[j][k];
		return a;
	}
	vector<T> operator*(const vector<T>& vec) const {
		vector<T> ret(N);
		rep(i,0,N) rep(j,0,N) ret[i] += d[i][j] * vec[j];
		return ret;
	}
	Matrix<T, N, N> operator^(ll p) const {
		if constexpr (N != M) throw std::invalid_argument("Matrix is not square");
		assert(p >= 0);
		Matrix<T, N, N> a, b(*this);
		rep(i,0,N) a.d[i][i] = 1;
		while (p) {
			if (p&1) a = a*b;
			b = b*b;
			p >>= 1;
		}
		return a;
	}
};
