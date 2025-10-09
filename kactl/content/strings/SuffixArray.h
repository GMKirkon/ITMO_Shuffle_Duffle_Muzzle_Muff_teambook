struct sparse {
    vector<vector<int>> st;
    sparse() { }
    sparse(const vector<int> &a) {
        int n = a.size();
        int k = 1;
        while (1 << k <= n)
            k++;
        st.resize(k, vector<int>(n));
        st[0] = a;
        for (int i = 1; i < k; i++) {
            for (int j = 0; j + (1 << i) <= n; j++)
                st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
        }
    }
    int getMin(int l, int r) {
        int k = 31 - __builtin_clz(r - l);
        return min(st[k][l], st[k][r - (1 << k)]);
    }
};
struct SuffixArray {
    int n;
    vector<int> s, p, lcp, rev_p;
    sparse st;
    SuffixArray() { }
    SuffixArray(const vector<int>& s) : n(s.size()), s(s) {
        assert(!s.empty() && s.back() == *max_element(s.begin(), s.end()));
        p.assign(n, 0);
        vector<int> c(n);
        vector<vector<int>> pos(*max_element(s.begin(), s.end()) + 1);
        for (int i = 0; i < n; i++)
            pos[s[i]].push_back(i);
        int cnt = 0;
        int classes = 0;
        for (int x = 0; x < pos.size(); ++x) {
            for (int u : pos[x]) {
                c[u] = classes, p[cnt++] = u;
            }
            classes += !pos[x].empty();
        }
        vector<int> new_c(n);
        vector<vector<int> > a(classes);
        for (int pw2 = 1; classes < n; pw2++) {
            cnt = 0;
            for (int i = 0; i < a.size(); ++i) a[i].clear();
            a.insert(a.end(), classes - a.size(), vector<int>());
            for (int i = 0; i < n; i++) {
                int j = (p[i] - (1 << (pw2 - 1)) + n) % n;
                a[c[j]].push_back(j);
            }
            int new_classes = 0;
            for (int i = 0; i < classes; i++) {
                for (int j = 0; j < a[i].size(); j++) {
                    if (j == 0 || c[(a[i][j] + (1 << (pw2 - 1))) % n] != c[(a[i][j - 1] + (1 << (pw2 - 1))) % n]) {
                        new_c[a[i][j]] = new_classes;
                        new_classes++;
                    } else {
                        new_c[a[i][j]] = new_classes - 1;
                    }
                    p[cnt] = a[i][j];
                    ++cnt;
                }
            }
            c.swap(new_c);
            classes = new_classes;
        }
        lcp.assign(n, 0);
        rev_p.assign(n, 0);
        for (int i = 0; i < n; i++)
            rev_p[p[i]] = i;
        int k = 0;
        for (int i = 0; i < n; i++) {
            if (rev_p[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = p[rev_p[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k])
                k++;
            lcp[rev_p[i]] = k;
            if (k > 0)
                k--;
        }
        st = sparse(lcp);
    }
    int getLcp(int i, int j) {
        i = rev_p[i];
        j = rev_p[j];
        if (i > j)
            swap(i, j);
        return st.getMin(i, j);
    }
    bool compare(int i, int j) {
        if (i == j) return false;
        int k = getLcp(i, j);
        return s[i+k] < s[j+k];
    }
};
