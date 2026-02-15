17   int n = 100;
 18   vector <int> a(n);
 19   for (int i = 0; i < n; i++) {
 20     a[i] = rnd() % 101;
 21   }   
 22   for (int p = 1; p <= n; p *= 2) {
 23     for (int k = p; k >= 1; k /= 2) {
 24       vector <int> cnt(n);
 25       for (int j = k % p; j <= n - 1 - k; j += 2 * k) {
 26         for (int i = 0; i <= min(k - 1, n - j - k - 1); i++) {
 27           if ((i + j) / (2 * p) == (i + j + k) / (2 * p)) {
 28             int u = i + j, v = i + j + k;
 29             cnt[u]++, cnt[v]++;
 30             if (a[u] > a[v]) {
 31               swap(a[u], a[v]);
 32             }
 33           }
 34         }
 35       }
 36     }
 37   } 
