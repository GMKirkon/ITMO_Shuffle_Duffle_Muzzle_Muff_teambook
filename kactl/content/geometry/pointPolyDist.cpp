/**
 * Author: andreyDagger
 * Date: 2025-08-24
 * License: CC0
 * Description: Given convex polygon p ordered ccw and point z, finds distance from z to p. Assumes that p strictly outside. Requires some trivial geometry functions
 * Status: tested on https://acm.timus.ru/problem.aspx?space=1&num=2196
 */

template <typename PT>
inline int orientation(PT a, PT b, PT c) { return sgn((b - a).cross(c - a)); }

template <typename PT>
pair<PT, int> point_poly_tangent(vector<PT> &p, PT Q, int dir, int l, int r) {
    while (r - l > 1) {
        int mid = (l + r) >> 1;
        bool pvs = orientation(Q, p[mid], p[mid - 1]) != -dir;
        bool nxt = orientation(Q, p[mid], p[mid + 1]) != -dir;
        if (pvs && nxt) return {p[mid], mid};
        if (!(pvs || nxt)) {
            auto p1 = point_poly_tangent(p, Q, dir, mid + 1, r);
            auto p2 = point_poly_tangent(p, Q, dir, l, mid - 1);
            return orientation(Q, p1.first, p2.first) == dir ? p1 : p2;
        }
        if (!pvs) {
            if (orientation(Q, p[mid], p[l]) == dir)  r = mid - 1;
            else if (orientation(Q, p[l], p[r]) == dir) r = mid - 1;
            else l = mid + 1;
        }
        if (!nxt) {
            if (orientation(Q, p[mid], p[l]) == dir)  l = mid + 1;
            else if (orientation(Q, p[l], p[r]) == dir) r = mid - 1;
            else l = mid + 1;
        }
    }
    pair<PT, int> ret = {p[l], l};
    for (int i = l + 1; i <= r; i++) ret = orientation(Q, ret.first, p[i]) != dir ? make_pair(p[i], i) : ret;
    return ret;
}

template <typename PT>
bool on_seg(PT a, PT b, PT q) {
    if (orientation(a, b, q)) return false;
    return min(a.x, b.x) <= q.x && q.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= q.y && q.y <= max(a.y, b.y);
}

template <typename PT>
bool same_point(PT a, PT b) {
    return a.x == b.x && a.y == b.y;
}

template <typename PT>
pair<int, int> boundary_tangents(vector<PT> &p, PT Q) {
    int n = p.size();
    int s = orientation(p[0], p[1], p[n - 1]);

    auto vertex = [&](int v) {
        int l = (v + n - 1) % n;
        int r = (v + 1) % n;
        return s == 1 ? make_pair(r, l) : make_pair(l, r);
    };

    auto edge = [&](int l, int r) {
        return s == 1 ? make_pair(r, l) : make_pair(l, r);
    };

    if (same_point(Q, p[0])) return vertex(0);

    if (on_seg(p[0], p[1], Q)) {
        if (same_point(Q, p[1])) return vertex(1);
        return edge(0, 1);
    }

    if (on_seg(p[n - 1], p[0], Q)) {
        if (same_point(Q, p[n - 1])) return vertex(n - 1);
        return edge(n - 1, 0);
    }

    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) >> 1;
        if (orientation(p[0], p[mid], Q) * s >= 0) l = mid;
        else r = mid;
    }

    if (!on_seg(p[l], p[r], Q)) return {-1, -1};
    if (same_point(Q, p[l])) return vertex(l);
    if (same_point(Q, p[r])) return vertex(r);
    return edge(l, r);
}

template <typename PT>
pair<int, int> tangents_from_point_to_polygon(vector<PT> &p, PT Q) {
    // Remove this if you know that point doesn't lie on boundary
    auto b = boundary_tangents(p, Q);
    if (b.first != -1) return b;

    int ccw = point_poly_tangent(p, Q, 1, 0, (int)p.size() - 1).second;
    int cw = point_poly_tangent(p, Q, -1, 0, (int)p.size() - 1).second;
    return {ccw, cw};
}

// minimum distance from a point to a convex polygon
// it assumes point lie strictly outside the polygon
template <typename PT>
double dist_from_point_to_polygon(vector<PT> &p, PT z) {
    double ans = inf;
    int n = p.size();
    if (n <= 3) {
        for(int i = 0; i < n; i++) ans = min(ans, dist_from_point_to_seg(p[i], p[(i + 1) % n], z));
        return ans;
    }
    auto [r, l] = tangents_from_point_to_polygon(p, z);
    if(l > r) r += n;
    while (l < r) {
        int mid = (l + r) >> 1;
        double left = dist2(p[mid % n], z), right= dist2(p[(mid + 1) % n], z);
        ans = min({ans, left, right});
        if(left < right) r = mid;
        else l = mid + 1;
    }
    ans = sqrt(ans);
    ans = min(ans, dist_from_point_to_seg(p[l % n], p[(l + 1) % n], z));
    ans = min(ans, dist_from_point_to_seg(p[l % n], p[(l - 1 + n) % n], z));
    return ans;
}
