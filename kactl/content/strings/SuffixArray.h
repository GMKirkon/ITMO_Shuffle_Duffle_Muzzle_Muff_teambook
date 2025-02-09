/**
 * Author: 罗穗骞, chilli
 * Date: 2019-04-11
 * License: Unknown
 * Source: Suffix array - a powerful tool for dealing with strings
 * (Chinese IOI National team training paper, 2009)
 * Description: Builds suffix array for a string.
 * \texttt{sa[i]} is the starting index of the suffix which
 * is $i$'th in the sorted suffix array.
 * The returned vector is of size $n+1$, and \texttt{sa[0] = n}.
 * The \texttt{lcp} array contains longest common prefixes for
 * neighbouring strings in the suffix array:
 * \texttt{lcp[i] = lcp(sa[i], sa[i-1])}, \texttt{lcp[0] = 0}.
 * The input string must not contain any zero bytes.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once
struct sparse {
    vector<vector<int>> st;

    sparse() { }

    sparse(const vector<int> &a) {
        int n = Size(a);
        int k = 0;
        while (1 << k < n)
            k++;
        st.resize(k + 1, vector<int>(n));
        copy(all(a), st[0].begin());
        for (int i = 1; i <= k; i++) {
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
    vector<int> sa, lcp, pos;
    sparse st;
    vector<int> s;

    // O(Size(s) + max(s) - min(s))
    SuffixArray(vector<int> &s): n(Size(s)) {
        int mn = *min_element(all(s));
        for (int &i : s)
            i -= mn - 1;
        s.reserve(Size(s) + 1);
        s.push_back(0);
        sa = build(s, *max_element(all(s)) + 1);

        int n = Size(s);
        pos.resize(n);
        for (int i = 0; i < n; i++)
            pos[sa[i]] = i;
        lcp.resize(n);
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int j = sa[pos[i] - 1];
            while (s[i + k] == s[j + k])
                k++;
            lcp[pos[i]] = k;
            k = max(0, k - 1);
        }
        st = sparse(lcp);
        this->s = s;
    }

    vector<int> phase2(const vector<int> &s, const vector<int> &pref, const vector<char> &types, const vector<int> &lms) {
        int n = Size(s);
        vector<int> cnt = pref;
        vector<int> res(n, -1);
        for (int i : lms) {
            int a = s[i];
            res[--cnt[a + 1]] = i;
        }
        copy(all(pref), cnt.begin());
        for (int p : res) {
            if (p <= 0 || types[p - 1] != 'L')
                continue;
            int a = s[p - 1];
            res[cnt[a]++] = p - 1;
        }
        copy(all(pref), cnt.begin());
        for (int i = n - 1; i >= 0; i--) {
            int p = res[i];
            if (p <= 0 || types[p - 1] != 'S')
                continue;
            int a = s[p - 1];
            res[--cnt[a + 1]] = p - 1;
        }
        return res;
    }

    inline bool is_lms(const vector<char> &types, int i) {
        return types[i - 1] == 'L' && types[i] == 'S';
    }

    // compare two lms substring
    inline bool not_equal(const vector<int> &s, const vector<char> &types, int i, int j) {
        assert(is_lms(types, i) && is_lms(types, j));
        bool is_lms1 = false, is_lms2 = false;
        while (true) {
            if (s[i] != s[j] || types[i] != types[j])
                return true;
            if (is_lms1 && is_lms2)
                break;
            i++;
            j++;
            is_lms1 = is_lms(types, i);
            is_lms2 = is_lms(types, j);
        }
        return false;
    }

    // m = max(s) + 1, s.back() == 0
    vector<int> build(vector<int> &s, int m) {
        int n = Size(s);
        assert(!s.empty());
        assert(s.back() == 0);
        assert(Size(s) == 1 || *min_element(s.begin(), s.end() - 1) > 0);
        assert(*max_element(all(s)) == m - 1);
        if (Size(s) == 1)
            return {0};

        vector<char> types(n);
        types[n - 1] = 'S';
        vector<int> lms;
        lms.reserve(n);
        for (int i = n - 2; i >= 0; i--) {
            if (s[i] < s[i + 1])
                types[i] = 'S';
            else if (s[i] > s[i + 1])
                types[i] = 'L';
            else
                types[i] = types[i + 1];
            if (types[i] == 'L' && types[i + 1] == 'S')
                lms.push_back(i + 1);
        }
        vector<int> pref(m + 1);
        for (int i : s)
            pref[i + 1]++;
        for (int i = 0; i < m; i++)
            pref[i + 1] += pref[i];
        auto res = phase2(s, pref, types, lms);

        int lms_cnt = 1, color = 0;
        int last = n - 1;
        vector<int> new_sym(n, -1);
        new_sym[n - 1] = 0;
        for (int i = 1; i < n; i++) {
            int p = res[i];
            if (p <= 0 || !is_lms(types, p))
                continue;
            lms[lms_cnt++] = p;
            color += not_equal(s, types, last, p);
            new_sym[p] = color;
            last = p;
        }
        vector<int> new_string;
        vector<int> pos_new_string(n);
        new_string.reserve(Size(lms) + 1);
        for (int i = 0; i < n; i++) {
            int c = new_sym[i];
            if (c != -1) {
                pos_new_string[Size(new_string)] = i;
                new_string.push_back(c);
            }
        }
        if (color != Size(lms)) {
            auto sa_new = build(new_string, color + 1);
            for (int i = 1; i < Size(sa_new); i++)
                lms[i] = pos_new_string[sa_new[i]];
        }
        return phase2(s, pref, types, lms);
    }

    int get_lcp(int i, int j) {
        if (i == j)
            return n - i;
        i = pos[i];
        j = pos[j];
        if (i > j)
            swap(i, j);
        return st.getMin(i + 1, j + 1);
    }

    bool compare(int i, int j) {  // s[i..] < s[j..]
        if (i == j)
            return false;
        int k = get_lcp(i, j);
        return s[i + k] < s[j + k];
    }
};


//Another impl
struct SuffixArray {
	vi sa, lcp;
	SuffixArray(string& s, int lim=256) { // or basic_string<int>
		int n = sz(s) + 1, k = 0, a, b;
		vi x(all(s)+1), y(n), ws(max(n, lim)), rank(n);
		sa = lcp = y, iota(all(sa), 0);
		for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
			p = j, iota(all(y), n - j);
			rep(i,0,n) if (sa[i] >= j) y[p++] = sa[i] - j;
			fill(all(ws), 0);
			rep(i,0,n) ws[x[i]]++;
			rep(i,1,lim) ws[i] += ws[i - 1];
			for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
			swap(x, y), p = 1, x[sa[0]] = 0;
			rep(i,1,n) a = sa[i - 1], b = sa[i], x[b] =
				(y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
		}
		rep(i,1,n) rank[sa[i]] = i;
		for (int i = 0, j; i < n - 1; lcp[rank[i++]] = k)
			for (k && k--, j = sa[rank[i] - 1];
					s[i + k] == s[j + k]; k++);
	}
};
