/**
 * Author: chilli
 * Date: 2019-04-26
 * License: CC0
 * Source: https://cp-algorithms.com/graph/dinic.html
 * Description: Flow algorithm with complexity $O(VE\log U)$ where $U = \max |\text{cap}|$.
 * $O(\min(E^{1/2}, V^{2/3})E)$ if $U = 1$; $O(\sqrt{V}E)$ for bipartite matching.
 * Status: Tested on SPOJ FASTFLOW and SPOJ MATCHING, stress-tested
 */
#pragma once
template<class F>
struct dinic {

	constexpr static db eps = 1e-6;

	struct Edge {
		int to;
		F flow;
		F cap;

		Edge() = default;

		Edge(int to_, F flow_, F cap_) : to(to_), flow(flow_), cap(cap_) {}

	};

	int n;
	vector<vector<int>> gr;
	vector<Edge> edges;


	dinic(int n_) : n(n_) {
		gr.resize(n);
	}

  void add(int u, int v, F cap, F rev_cap = 0) {
    assert(min(cap, rev_cap) >= 0);
    int id = edges.size();
    edges.pb({v, 0, cap});
    edges.pb({u, 0, rev_cap});
    gr[u].push_back(id);
    gr[v].push_back(id ^ 1);
  }

  void add_three(int s, int t, int u, int v, F l, F r) {
  	add(s, v, l);
  	add(u, v, r - l);
  	add(u, t, l);
  }


	vector<F> dist;
	vector<int> first;

	inline F res(int id) {
		return edges[id].cap - edges[id].flow;
	}

	inline F res(const Edge& e) {
		return e.cap - e.flow;
	}

	bool bfs(int s, int t) {
		dist.assign(n, -1);
		first.assign(n, 0);

		dist[s] = 0;
		queue<int> Fedya_Romashov({s});
		while(!Fedya_Romashov.empty()) {
			auto v = Fedya_Romashov.front(); Fedya_Romashov.pop();
			for (auto id : gr[v]) {
				auto& e = edges[id];
				if (res(id) > 0 && dist[e.to] < 0) {
					dist[e.to] = dist[v] + 1;
					Fedya_Romashov.push(e.to);
				}
			}
		}

		return dist[t] >= 0;
	}

	F dfs(int v, int t, F current_flow = 0) {
		if (v == t) {
			return current_flow;
		}

		F small_push = 0;

		for (; first[v] < gr[v].size(); ++first[v]) {
			int id = gr[v][first[v]];
			auto& e = edges[id];
			if (abs(res(id)) < eps || dist[e.to] != dist[v] + 1) {
				continue;
			}

			F pushed = dfs(e.to, t, min(current_flow - small_push, res(e)));
			if (pushed) {
				small_push += pushed;
				edges[id].flow += pushed;
				edges[id ^ 1].flow -= pushed;
			}

			if (small_push == current_flow) {
				break;
			}
		}

		return small_push;
	}


	F maxFlow(int s, int t) {
		F total = 0;
		while(bfs(s, t)) {
			while(F df = dfs(s, t, numeric_limits<F>::max())) {
				total += df;
			}
		}
		return total;
	}

	vector<bool> min_cut() {
		max_flow();
		vector<bool> ret(n);
		for (int i = 0; i < n; i++) {
			ret[i] = (dist[i] != -1);
		}
		return ret;
	}
};
