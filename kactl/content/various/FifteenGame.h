/**
 * Author: Andrey Matveev
 * Date: 2025-12
 * License: CC0
 * Source: DeepSeek
 * Description: Checking solvability of "game in 15"
 */

bool solvable(vector<vector<int>> a) {
    int n = a.size(), m = a[0].size();
    int r = -1;
    vector<int> b;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 0) r = n - i;
            else b.push_back(a[i][j]);
        }
    }
    int inv = 0;
    for (int i = 0; i < b.size(); ++i) for (int j = 0; j < i; ++j) inv += (b[j] > b[i]);
    if (m % 2 != 0) return inv % 2 == 0;
    return inv % 2 != r % 2;
}
