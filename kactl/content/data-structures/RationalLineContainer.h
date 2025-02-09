/**
 * Author: Dmitry Ivchenko
 * Date: 2024-12-12
 * License: CC0
 * Description: CHT with rationals
 * Time: O(\log N)
 */
struct CHT {
    struct Frac {
        ll a, b;

        bool operator <(const Frac &other) const {
            return (__int128_t)a * other.b < (__int128_t)other.a * b;
        }

        bool operator >(const Frac &other) const {
            return other < *this;
        }
    };

    struct Line {
        ll k, b;

        ll get(ll x) const {
            return k * x + b;
        }

        Frac intersect(const Line &other) const {
            Frac res = {other.b - b, k - other.k};
            if (res.b < 0) {
                res.a *= -1;
                res.b *= -1;
            }
            return res;
        }
    };

    vector<Frac> pt;
    vector<Line> lines;

    void addLine(ll k, ll b) {  // k[i] descrease
        Line l = {k, b};
        while (!pt.empty()) {
            const Line &back = lines.back();
            if (back.k == k) {
                if (b >= back.b)
                    return;
            } else if (l.intersect(back) > pt.back()) {
                break;
            }
            pt.pop_back();
            lines.pop_back();
        }
        if (!lines.empty() && lines.back().k == k) {
            if (b < lines.back().b)
                lines.back() = l;
            return;
        }
        if (!lines.empty())
            pt.push_back(l.intersect(lines.back()));
        lines.push_back(l);
    }

    ll getMin(ll x) {
        if (lines.empty())
            return INT64_MAX;
        int pos = lower_bound(all(pt), Frac{x, 1}) - pt.begin();
        return lines[pos].get(x);
    }
};
