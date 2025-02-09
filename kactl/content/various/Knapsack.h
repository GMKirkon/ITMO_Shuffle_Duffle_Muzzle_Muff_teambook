/**
 * Author: dimms
 * Date: 2024
 * License: CC0
 * Source: telegram
 * Description: Knapsack fast.
 * Time: O(n^2 C log n / 64) and O(nC / 64) 
 * Status: works
 */

#pragma push_macro("__SIZEOF_LONG__")
#pragma push_macro("__cplusplus")
#define __SIZEOF_LONG__ __SIZEOF_LONG_LONG__
#define unsigned unsigned long
#define __cplusplus 201102L

#define __builtin_popcountl __builtin_popcountll
#define __builtin_ctzl __builtin_ctzll

#pragma pop_macro("__cplusplus")
#pragma pop_macro("__SIZEOF_LONG__")
#undef unsigned
#undef __builtin_popcountl
#undef __builtin_ctzl

const int C = 1e6 + 3;

vector<int> ans;
int M;
bitset<C> dp1, dp2;

bool divide(const vector<int> &a, int l, int r, int S) {
    if (r - l == 1) {
        if (a[l] == S) {
            ans.push_back(l);
        } else if (S != 0) {
            return false;
        }
        return true;
    }
    int m = (l + r) >> 1;
    dp1 = 0;
    dp1[0] = true;
    for (int i = l; i < m; i++)
        dp1 |= dp1 << a[i];
    dp2 = 0;
    dp2[S] = true;
    for (int i = r - 1; i >= m; i--)
        dp2 |= dp2 >> a[i];
    for (int x = 0; x <= (r - l) * M; x++) {
        if (dp1[x] && dp2[x]) {
            assert(divide(a, l, m, x));
            assert(divide(a, m, r, S - x));
            return true;
        }
    }
    return false;
}
