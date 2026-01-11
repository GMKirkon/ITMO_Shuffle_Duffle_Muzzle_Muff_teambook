#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <iomanip>

using namespace std;

struct Point {
    double x, y, z;
};

vector<Point> points;
int N;

// Вращение точки: сначала вокруг Z на a1, затем вокруг Y на a2, затем вокруг X на a3
Point rotate_point(double x, double y, double z, double a1, double a2, double a3) {
    // 1. Вращение вокруг Z
    double x1 = x * cos(a1) - y * sin(a1);
    double y1 = x * sin(a1) + y * cos(a1);
    double z1 = z;

    // 2. Вращение вокруг Y
    double x2 = x1 * cos(a2) + z1 * sin(a2);
    double z2 = -x1 * sin(a2) + z1 * cos(a2);
    double y2 = y1;

    // 3. Вращение вокруг X
    double y3 = y2 * cos(a3) - z2 * sin(a3);
    double z3 = y2 * sin(a3) + z2 * cos(a3);
    double x3 = x2;

    return {x3, y3, z3};
}

// Функция качества: дисперсия диапазонов проекций на три оси куба
double f(double a1, double a2, double a3) {
    Point v1 = rotate_point(1, 0, 0, a1, a2, a3);
    Point v2 = rotate_point(0, 1, 0, a1, a2, a3);
    Point v3 = rotate_point(0, 0, 1, a1, a2, a3);

    vector<double> p1, p2, p3;
    for (auto& p : points) {
        p1.push_back(p.x * v1.x + p.y * v1.y + p.z * v1.z);
        p2.push_back(p.x * v2.x + p.y * v2.y + p.z * v2.z);
        p3.push_back(p.x * v3.x + p.y * v3.y + p.z * v3.z);
    }

    auto range = [](const vector<double>& v) {
        auto mm = minmax_element(v.begin(), v.end());
        return *(mm.second) - *(mm.first);
    };

    double r1 = range(p1);
    double r2 = range(p2);
    double r3 = range(p3);
    double avg = (r1 + r2 + r3) / 3.0;
    return (r1 - avg) * (r1 - avg) + (r2 - avg) * (r2 - avg) + (r3 - avg) * (r3 - avg);
}

int main() {
#ifdef ONPC
    freopen("a.in", "r", stdin);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    points.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> points[i].x >> points[i].y >> points[i].z;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 2.0 * acos(-1.0));

restart:

    double a1 = dis(gen);
    double a2 = dis(gen);
    double a3 = dis(gen);
    double best_val = f(a1, a2, a3);

    double step = acos(-1.0); // начальный шаг = π
    const double alpha = 0.9; // уменьшение шага
    const int max_iter = 15000;

    for (int iter = 0; iter < max_iter; ++iter) {
        bool improved = false;

        // Генерируем соседей: меняем каждый угол на ±step
        vector<tuple<double, double, double>> candidates;
        for (int da1 : {-1, 0, 1}) {
            for (int da2 : {-1, 0, 1}) {
                for (int da3 : {-1, 0, 1}) {
                    if (da1 == 0 && da2 == 0 && da3 == 0) continue;
                    candidates.emplace_back(a1 + da1 * step, a2 + da2 * step, a3 + da3 * step);
                }
            }
        }

        // Перемешиваем для случайности (опционально)
        shuffle(candidates.begin(), candidates.end(), gen);

        for (auto [na1, na2, na3] : candidates) {
            double val = f(na1, na2, na3);
            if (val < best_val) {
                best_val = val;
                a1 = na1;
                a2 = na2;
                a3 = na3;
                improved = true;
                break; // жадный выбор первого улучшения
            }
        }

        if (!improved) {
            step *= alpha;
            if (step < 1e-14) break;
        }

        if (best_val < 1e-20) break;
    }

    if (best_val > 1e-9) goto restart;

    // Получаем базис
    Point v1 = rotate_point(1, 0, 0, a1, a2, a3);
    Point v2 = rotate_point(0, 1, 0, a1, a2, a3);
    Point v3 = rotate_point(0, 0, 1, a1, a2, a3);

    auto norm = [](Point p) {
        double len = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
        if (len < 1e-15) return Point{0, 0, 0};
        return Point{p.x / len, p.y / len, p.z / len};
    };

    v1 = norm(v1);
    v2 = norm(v2);
    v3 = norm(v3);

    cout << fixed << setprecision(30);
    cout << v1.x << " " << v1.y << " " << v1.z << "\n";
    cout << v2.x << " " << v2.y << " " << v2.z << "\n";
    cout << v3.x << " " << v3.y << " " << v3.z << endl;

    return 0;
}
