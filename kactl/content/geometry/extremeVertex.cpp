/**
 * Author: andreyDagger
 * Date: 2025-04-12
 * License: CC0
 * Description: Given convex polygon p ordered ccw and point z, finds vertex of polygon w, such that dot(w, z) is maximum. top - upper right vertex. Needs any adequate implementation of PT structure
 * Status: tested on https://codeforces.com/gym/105811/problem/E
 */
inline int dot(PT a, PT b) { return a.x * b.x + a.y * b.y; }

int extreme_vertex(vector<PT> &p, const PT &z, const int top) {
    int n = p.size();
    if (n == 1) return 0;
    int ans = dot(p[0], z); int id = 0;
    if (dot(p[top], z) > ans) ans = dot(p[top], z), id = top;
    int l = 1, r = top - 1;
    while (l < r) {
        int mid = l + r >> 1;
        if (dot(p[mid + 1], z) >= dot(p[mid], z)) l = mid + 1;
        else r = mid;
    }
    if (dot(p[l], z) > ans) ans = dot(p[l], z), id = l;
    l = top + 1, r = n - 1;
    while (l < r) {
        int mid = l + r >> 1;
        if (dot(p[(mid + 1) % n], z) >= dot(p[mid], z)) l = mid + 1;
        else r = mid;
    }
    l %= n;
    if (dot(p[l], z) > ans) ans = dot(p[l], z), id = l;
    return id;
}