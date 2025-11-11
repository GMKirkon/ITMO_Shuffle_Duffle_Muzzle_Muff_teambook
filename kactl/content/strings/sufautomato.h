#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma once

using namespace std;


struct state {
	int len, link;
  int first_pos;
  bool is_clone;
	map<char,int> next;
};

const int MAXLEN = (int) (1e6) + 4;
state st[MAXLEN*2];
int sz, last;

void sa_init() {
	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
	for (int i=0; i<MAXLEN*2; ++i) {
		st[i].next.clear();
    st[i].is_clone = false;
  }
}

int me = 0;

void sa_extend (char c) {
	int cur = sz++;
	st[cur].len = st[last].len + 1;
  st[cur].first_pos = me;
  me++;
	int p;
	for (p=last; p!=-1 && !st[p].next.count(c); p=st[p].link)
		st[p].next[c] = cur;
	if (p == -1)
		st[cur].link = 0;
	else {
		int q = st[p].next[c];
		if (st[p].len + 1 == st[q].len)
			st[cur].link = q;
		else {
			int clone = sz++;
      st[clone].is_clone = true;
      st[clone].first_pos = st[p].first_pos;//v EMAXXE napisno q, no ne ispolzuetsya, tak kak is clone
			st[clone].len = st[p].len + 1;
			st[clone].next = st[q].next;
			st[clone].link = st[q].link;
			for (; p!=-1 && st[p].next[c]==q; p=st[p].link)
				st[p].next[c] = clone;
			st[q].link = st[cur].link = clone;
		}
	}
	last = cur;
}

void calculate_occurences() {
  vector <int> ord;
  for (int i = 0; i < sz; i++) {
    ord.push_back(i);
  }
  sort(ord.begin(), ord.end(), [&] (int i, int j) {
    return st[i].len > st[j].len;
  });
  for (int v : ord) {
    dp[v] += !st[v].is_clone;
    if (!st[v].is_clone) {
      rdp[v] += pals_start[st[v].first_pos + 1];
    }
    if (v) {
      rdp[st[v].link] += rdp[v];
      dp[st[v].link] += dp[v];
    }
  }
}
