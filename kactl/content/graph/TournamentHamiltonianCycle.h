/**
 * Author: Andrey Matveev
 * Date: 2025-06-04
 * License: CC0
 * Source: https://github.com/B-O-O-P/itmo-dm-labs/blob/master/term%203/Hamilton%20cycles/D.cpp
 * Description: Finds hamiltonian cycle in strong connected tournament
 * Time: O(N^2)
 * Status: AC on "2017 Benelux Algorithm Programming Contest (BAPC 17). Problem K"
 */

struct tournament_hamiltonian_cycle {
    int n;
    vector<vector<bool>> graph;
    vector<int> path, cycle;
public:
    tournament_hamiltonian_cycle(const vector<vector<int>>& mat) : n(mat.size()) {
        graph.resize(n);
        for (int i = 0; i < n; ++i) {
            graph[i].resize(n);
            for (int j = 0; j < n; ++j) {
                graph[i][j] = mat[i][j];
            }
        }
    }

    vector<int> getHamiltonianCycle() {
        getHamPath();
        getHamCycle();
        PathIntoCycle();
        return cycle;
    }
private:
    void getHamPath() {
        for (int i = 0; i < n; ++i) {
            if (path.empty()) {
                path.push_back(i);
            } else {
                auto item = path.begin();
                while (item != path.end() && graph[*item][i]) {
                    ++item;
                }
                path.insert(item, i);
            }
        }
    }

    void getHamCycle() {
        int st = path[0];
        int pos = 0;
        for (int i = path.size() - 1; i >= 2; --i) {
            if (graph[path[i]][st]) {
                pos = i;
                break;
            }
        }
        cycle.insert(cycle.begin(), path.begin(), path.begin() + pos + 1);
        path.erase(path.begin(), path.begin() + pos + 1);
    }

    void PathIntoCycle() {
        for (auto st = path.begin(); st != path.end();) {
            auto item = cycle.begin();
            while (item != cycle.end() && graph[*item][*st]) {
                item++;
            }
            if (item != cycle.end()) {
                cycle.insert(item, path.begin(), st + 1);
                path.erase(path.begin(), st + 1);
                st = path.begin();
            } else {
                st++;
            }
        }
    }
};
