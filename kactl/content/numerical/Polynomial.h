/**
 * Author: AndreyDagger
 * Date: 2024-12-12
 * Description: Polynomial operations
 */

namespace Polynomial {

    template<typename base>
    vector<base> derivative(vector<base> a) {
        int n = a.size();
        for (int i = 0; i < n - 1; ++i) {
            a[i] = a[i + 1] * (i + 1);
        }
        a.pop_back();
        return a;
    }

    template<typename base>
    vector<base> integral(vector<base> a) {
        int n = a.size();
        a.push_back(0);
        for (int i = n; i > 0; --i) {
            a[i] = a[i - 1] / i;
        }
        a[0] = 0;
        return a;
    }

    template<typename base>
    vector<base> add(vector<base> a, const vector<base> &b) {
        int n = a.size(), m = b.size();
        a.resize(max(n, m));
        for (int i = 0; i < max(n, m); ++i) {
            a[i] = (i >= a.size() ? 0 : a[i]) + (i >= b.size() ? 0 : b[i]);
        }
        return a;
    }

    template<typename base>
    vector<base> sub(vector<base> a, const vector<base> &b) {
        int n = a.size(), m = b.size();
        a.resize(max(n, m));
        for (int i = 0; i < max(n, m); ++i) {
            a[i] = (i >= a.size() ? 0 : a[i]) - (i >= b.size() ? 0 : b[i]);
        }
        return a;
    }

    namespace NTT {
        const int MOD = 998244353;
        const int g = 3;

        vector<int> R;

        void NTT(vector<Mint<MOD>>& a, int n, int on) {
            for (int i = 0; i < n; i++)
                if (i < R[i])
                    swap(a[i], a[R[i]]);
            Mint<MOD> wn, u, v;
            for (int i = 1, m = 2; i < n; i = m, m <<= 1) {
                wn = Mint<MOD>::binpow(g, (MOD - 1) / m);
                if (on == -1)
                    wn = 1 / wn;
                for (int j = 0; j < n; j += m) {
                    Mint<MOD> w = 1;
                    for (int k = 0; k < i; k++, w *= wn) {
                        u = a[j + k], v = w * a[i + j + k];
                        a[j + k] = u + v;
                        a[i + j + k] = u - v;
                    }
                }
            }
            if (on == -1) {
                Mint<MOD> k = Mint<MOD>(1) / Mint<MOD>(n);
                for (int i = 0; i < n; i++)
                    a[i] = a[i] * k;
            }
        }

        template<typename base>
        vector<base> mul(vector<base>& A, vector<base>& B) {
            static_assert(std::is_same_v<base, Mint<MOD>>);
            assert(A.size() == B.size() && __builtin_popcount(A.size()) == 1);
            int n = A.size();
            int L = __builtin_ctz(n);
            if (R.size() != n) {
                R.assign(n, 0);
                for (int i = 0; i < n; i++)
                    R[i] = (R[i >> 1] >> 1) | ((i & 1) << (L - 1));
            }
            NTT(A, n, 1);
            NTT(B, n, 1);
            for (int i = 0; i < n; i++)
                A[i] *= B[i];
            NTT(A, n, -1);
            return A;
        }
    }

    int get_lim(int n) {
        int res = 1;
        while (res < n) {
            res <<= 1;
        }
        return res;
    }

    template<typename base>
    vector<base> mul(vector<base> a, vector<base> b, int size) {
        int l = get_lim(a.size() + b.size());
        a.resize(l);
        b.resize(l);
        auto res = NTT::mul(a, b);
        res.resize(size);
        return res;
    }

    template<typename base>
    vector<base> mul(vector<base> a, base scalar) {
        for (auto& val : a)
            val *= scalar;
        return a;
    }

    template<typename base>
    vector<base> mul(const vector<base> &a, const vector<base> &b) {
        return mul(a, b, a.size() + b.size() - 1);
    }

    template <typename base>
    vector<base> plug_minus_x(vector<base> a) {
        for (int i = 1; i < a.size(); i += 2) {
            a[i] *= -1;
        }
        return a;
    }

    template <typename base>
    void plug_x_squared_inplace(vector<base>& a) {
        a.resize(a.size() * 2);
        for (int i = (int)a.size() * 2 - 1; i >= 0; --i) {
            if (i % 2 != 0) a[i] = 0;
            else a[i] = a[i / 2];
        }
    }

    template <typename base>
    vector<base> plug_x_squared(const vector<base>& a) {
        vector<base> res(a.size() * 2);
        for (int i = 0; i < a.size(); ++i) {
            res[i * 2] = a[i];
        }
        return res;
    }

    template <typename base>
    void only_even_inplace(vector<base>& a) {
        for (int i = 0; i < a.size(); i += 2) {
            a[i / 2] = a[i];
        }
        a.resize((a.size() + 1) / 2);
    }

    template <typename base>
    vector<base> only_even(const vector<base>& a) {
        vector<base> res((a.size() + 1) / 2);
        for (int i = 0; i < a.size(); i += 2) {
            res[i / 2] = a[i];
        }
        return res;
    }

    // O(n*log(n))
    template<typename base>
    void inverse_inplace(vector<base> &a, int size) {
        assert(!a.empty() && a[0] != 0);
        if (size == 0) {
            a = {0};
            return;
        }
        if (size == 1) {
            a = {1/a[0]};
            return;
        }
        auto op = plug_minus_x(a);
        auto T = mul(a, op);
        only_even_inplace(T);
        inverse_inplace(T, (size + 1) / 2);
        plug_x_squared_inplace(T);
        a = mul(op, T, size);
    }

    template <typename base>
    vector<base> inverse(const vector<base>& a, int size) {
        assert(size > 0 && a[0] != 0);
        vector<base> Q{1/a[0]};

        for (int sz = 2;; sz *= 2) {
            Q = mul(Q, sub({2}, mul(a, Q, sz)), sz);
            if (sz >= size)
                break;
        }
        Q.resize(size);
        return Q;
    }

//    // O(n*log(n)) too slow, big constant factor
//    template<typename base>
//    vector<base> inverse(const vector<base> &a, int size) {
//        assert(!a.empty() && a[0] != 0);
//        if (size == 0) {
//            return {0};
//        }
//        if (size == 1) {
//            return {1/a[0]};
//        }
//        auto op = plug_minus_x(a);
//        auto T = mul(a, op);
//        T = only_even(T);
//        T = inverse(T, (size + 1) / 2);
//        T = plug_x_squared(T);
//        auto res = mul(op, T, size);
//        return res;
//    }

    template<typename base>
    vector<base> divide(const vector<base> &a, const vector<base> &b, int size) {
        return mul(a, inverse(b, size), size);
    }

    // O(n*log(n))
    template<typename base>
    vector<base> ln(const vector<base> &a, int size) {
        auto res = integral(divide(derivative(a), a, size));
        res.resize(size);
        return res;
    }

    // O(n*log(n))
    template<typename base>
    vector<base> exp(const vector<base> &a, int size) {
        assert(size > 0 && a[0] == 0);
        vector<base> Q{1};

        for (int sz = 2;; sz *= 2) {
            Q = mul(Q, sub(add(a, {1}), ln(Q, sz)), sz);
            if (sz >= size)
                break;
        }
        Q.resize(size);
        return Q;
    }

    // O(n*log(n))
    template<typename base>
    vector<base> pow(vector<base> a, ll p, int size) {
        int i = 0;
        while (i < a.size()) {
            if (a[i] != 0)
                break;
            ++i;
        }
        if (i == a.size()) {
            auto res = vector<base>(size, 0);
            if (p == 0)
                res[0] = 1;
            return res;
        }
        a.erase(a.begin(), a.begin() + i);
        auto f = a[0];
        for (auto& x : a) x /= f;
        a = exp(mul(ln(a, size), (base)p), size);
        for (int j = size - 1; j >= 0; --j) {
            if ((i > 0 && p >= size) || j - p * i < 0)
                a[j] = 0;
            else
                a[j] = a[j - i * p];
            a[j] *= base::binpow(f, p);
        }
        return a;
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MOD = 998244353;
    int n; cin >> n;
    ll m; cin >> m;
    vector<Mint<MOD>> a(n);
    for (auto& x : a) cin >> x;
    auto res = Polynomial::pow(a, m, n);
    for (auto x : res) cout << x << " ";
}
