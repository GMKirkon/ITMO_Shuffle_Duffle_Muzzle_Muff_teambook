/**
 * Author: andreyDagger
 * Date: 2025-08-25
 * License: CC0
 * Source: https://ru.algorithmica.org/cs/persistent/persistent-segtree/
 * Description: persistent segtree
 * Time: $O(\log N)$.
 * Status: tested on https://official.contest.yandex.com/ptz-summer-2025/contest/79843/problems/K/
 */
struct SegtreeSum {
    int l, r, sum = 0;
    SegtreeSum* left = 0, *right = 0;
    SegtreeSum(int l , int r) : l(l), r(r) {
        int m = (l + r) / 2;
        if (r - l > 1) {
            left = new SegtreeSum(l, m);
            right = new SegtreeSum(m, r);
        }
    }
    void copyLeft() { if (left) left = new SegtreeSum(*left); }
    void copyRight() { if (right) right = new SegtreeSum(*right); }
    void add(int idx, int val) {
        sum += val;
        int m = (l + r) / 2;
        if (r - l > 1) {
            if (idx < m) {
                copyLeft();
                left->add(idx, val);
            } else {
                copyRight();
                right->add(idx, val);
            }
        }
    }
};
// init:
SegtreeSum* init_version = new SegtreeSum(0, n);
SegtreeSum* version_with_update = new SegtreeSum(*init_version);
version_with_update->add(4, 7);