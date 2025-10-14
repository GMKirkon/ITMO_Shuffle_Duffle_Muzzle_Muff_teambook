#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <list>
#include <time.h>
#include <math.h>
#include <random>
#include <deque>
#include <queue>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <bitset>
#include <sstream>
#include <chrono>

using namespace std;

typedef long long ll;

#ifdef ONPC
    mt19937 rnd(228);
#else
    mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
#endif

//fft
const int T = 2e6;
const int MOD = 998244353;
const ll MOD2 = MOD * (ll) MOD;
const ll root = 3;
const ll sub = 15311432;
const ll sub_inv = 469870224;

int st[T], rev[T];
int p_fft, w_fft, z_fft, s_fft, k_fft;

inline int power(int a, ll n)
{
    int ans = 1;
    while (n)
    {
        if (n & 1LL) ans = 1LL * ans * a % MOD;
        a = 1LL * a * a % MOD;
        n >>= 1LL;
    }
    return ans;
}

inline int inv(int x)
{
    return power(x, MOD - 2);
}

inline void fft(vector<int> &a)
{
    for (int i = 0; i < s_fft; i++)
        if (i < rev[i])
            swap(a[i], a[rev[i]]);
    for (int k = 1; k < s_fft; k <<= 1)
        for (int i = 0; i < s_fft; i += k + k)
            for (int j = i, t = i + k; j < i + k; j++, t++)
            {
                z_fft = 1LL * a[t] * st[t - i] % MOD;
                a[t] = a[j] - z_fft;
                if (a[t] < 0) a[t] += MOD;
                a[j] = a[j] + z_fft;
                if (a[j] >= MOD) a[j] -= MOD;
            }
}

inline vector<int> multiply(vector<int> a, vector<int> b)
{
    k_fft = ((int)a.size() + (int)b.size()) - 1;
    p_fft = 0;
    while ((1 << p_fft) < k_fft) p_fft++;
    s_fft = (1 << p_fft);
    for (int i = 0; i < s_fft; i++) rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (p_fft - 1));
    st[0] = 0;
    st[1] = 1;
    for (int b = 1; b < p_fft; b++)
    {
        w_fft = power(sub, 1 << (22 - b));
        for (int i = (1 << (b - 1)); i < (1 << b); i++)
        {
            st[i << 1] = st[i];
            st[(i << 1) + 1] = 1LL * st[i] * w_fft % MOD;
        }
    }
    while (a.size() < s_fft) a.push_back(0);
    while (b.size() < s_fft) b.push_back(0);
    fft(a);
    fft(b);
    for (int b = 1; b < p_fft; b++)
    {
        w_fft = power(sub_inv, 1 << (22 - b));
        for (int i = (1 << (b - 1)); i < (1 << b); i++)
        {
            st[i << 1] = st[i];
            st[(i << 1) + 1] = 1LL * st[i] * w_fft % MOD;
        }
    }
    vector<int> v;
    for (int i = 0; i < s_fft; i++) v.push_back(1LL * a[i] * b[i] % MOD);
    fft(v);
    while (v.back() == 0) v.pop_back();
    k_fft = inv(s_fft);
    for (int i = 0; i < (int)v.size(); i++) v[i] = 1LL * k_fft * v[i] % MOD;
    return v;
}

struct poly : vector<int>
{
    poly(const vector<int> &a) : vector<int>(a) {}
    poly(int n) : vector<int>(n, 1) {}

    inline void normalize()
    {
        while (size() > 1 && back() == 0) pop_back();
    }

    inline int& operator[](int pos)
    {
        if (pos >= (int)size()) resize(pos + 1);
        return vector<int>::operator[](pos);
    }

    inline int operator[](int pos) const
    {
        if (pos < (int)size()) return vector<int>::operator[](pos);
        return 0;
    }

    inline poly operator+(const poly &b) const
    {
        const poly &a = *this;
        poly ans(max((int)a.size(), (int)b.size()));
        for (int i = 0; i < ans.size(); i++)
        {
            ans[i] = (a[i] + b[i]);
            if (ans[i] >= MOD) ans[i] -= MOD;
        }
        return ans;
    }

    inline poly operator+=(const poly &p)
    {
        return *this = *this + p;
    }

    inline poly operator-(const poly &b) const
    {
        const poly &a = *this;
        poly ans(max((int)a.size(), (int)b.size()));
        for (int i = 0; i < ans.size(); i++)
        {
            ans[i] = (a[i] - b[i]);
            if (ans[i] < 0) ans[i] += MOD;
        }
        return ans;
    }

    inline poly operator-=(const poly &p)
    {
        return *this = *this - p;
    }

    inline poly operator*(int d) const
    {
        poly ans = *this;
        for (int i = 0; i < (int)ans.size(); i++)
            ans[i] = (1LL * ans[i] * d % MOD);
        return ans;
    }

    inline poly operator*=(int d)
    {
        return *this = *this * d;
    }

    inline poly operator/(int d) const
    {
        d = inv(d);
        poly ans = *this;
        for (int i = 0; i < (int)ans.size(); i++)
            ans[i] = (1LL * ans[i] * d % MOD);
        return ans;
    }

    inline poly operator/=(int d)
    {
        return *this = *this / d;
    }

    inline poly operator*(const poly &p) const
    {
        return poly(multiply(*this, p));
    }

    inline poly operator*=(const poly &p)
    {
        return *this = *this * p;
    }

    inline poly cut(int n) const
    {
        poly a = (*this);
        a.resize(n);
        return a;
    }

    inline poly invp() const
    {
        poly a(1);
        if ((*this)[0] != 1) a[0] = inv((*this)[0]);
        for (int n = 1; n < (int)size(); n <<= 1)
        {
            poly ca = cut(n + n);
            poly ra = (a * a);
            ra.cut(n + n);
            ra = (ra * ca);
            ra.cut(n + n);
            a += a;
            a -= ra;
            a = a.cut(n + n);
        }
        a.resize(size());
        return a;
    }

    inline poly rev() const
    {
        poly a = (*this);
        reverse(a.begin(), a.end());
        return a;
    }

    inline poly getdiv(poly b) const
    {
        poly a = (*this);
        a.normalize();
        b.normalize();
        if (a.size() < b.size()) return poly({0});
        int k = (int)a.size() - (int)b.size() + 1;
        if (b.back() != 1) b /= b.back();
        poly rb = b.rev().cut(k);
        return (rb.invp() * a.rev()).cut(k).rev();
    }

    inline poly operator/(const poly &b) const
    {
        return getdiv(b);
    }

    inline poly operator/=(const poly &b)
    {
        return *this = *this / b;
    }

    inline poly operator%(const poly &b) const
    {
        poly a = (*this);
        if (a.size() < b.size()) return a;
        poly r = (a - (a / b) * b);
        r.normalize();
        return r;
    }

    inline poly operator%=(const poly &b)
    {
        return *this = *this % b;
    }

    inline poly der() const
    {
        poly a = (*this);
        vector<int> ans;
        for (int i = 1; i < a.size(); i++)
            ans.push_back(1LL * i * a[i] % MOD);
        return poly(ans);
    }

    inline poly integral() const
    {
        poly a = (*this);
        vector<int> ans;
        ans.push_back(0);
        for (int i = 0; i < (int)a.size() - 1; i++)
            ans.push_back(1LL * inv(i + 1) * a[i] % MOD);
        return poly(ans);
    }
};

inline poly log(poly a)
{
    poly b = a;
    b = b.der();
    poly x = a.invp();
    b *= x;
    b.resize(a.size());
    b = b.integral();
    b[0] = 0;
    return b;
}

inline poly exp(poly f)
{
    poly a(1);
    for (int n = 1; n < (int)f.size(); n <<= 1)
    {
        while (a.size() < n + n) a.push_back(0);
        poly ca = f.cut(n + n);
        a += (ca - log(a)) * a;
        a = a.cut(n + n);
    }
    a.resize(f.size());
    return a;
};

int sign(int x)
{
    if (x % 2 == 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int main()
{
#ifdef ONPC
    freopen("a.in", "r", stdin);
#endif
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    poly ret(n + 10);
    for (int &x : ret) x = 0;
    for (int q = -n; q <= n; q++)
    {
        ll p = (3 * (ll) q * q - q) / 2;
        if (0 <= p && p < n + 10)
        {
            ret[p] += sign(q);
        }
    }
    auto solve = ret.invp();
    cout << (solve[n + 1] + MOD - 1) % MOD << '\n';
}
