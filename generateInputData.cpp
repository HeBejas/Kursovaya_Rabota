#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <locale>

using namespace std;

struct Point {
    int x, y;
};
struct Edge {
    int from;
    int to;
    double cost;
};

double distance(const Point& a, const Point& b) { // Расчет расстояния между двумя точками
    double dx = double(a.x) - double(b.x);
    double dy = double(a.y) - double(b.y);
    return sqrt(dx*dx + dy*dy);
}

void generateDenseEdges(const vector<Point>& V, long long M, mt19937& gen, ofstream& file){
    int N = V.size();
    set<pair<int,int>> used;
    long long written = 0;

    // Скелет для связности
    for (int i = 0; i < N - 1 && written < M; ++i) {
        int u = i, v = i + 1;
        if (u > v) swap(u, v);
        if (used.count({u, v})) continue;

        double w = distance(V[u], V[v]);
        file << u << " " << v << " " << w << "\n";
        used.insert({u, v});
        written++;
    }

     //Добиваем случайными рёбрами до M
    uniform_int_distribution<> pick(0, N - 1);

    while (written < M) {
        int a = pick(gen);
        int b = pick(gen);
        if (a == b) continue;

        int u = min(a, b);
        int v = max(a, b);

        if (used.count({u, v})) continue;

        double w = distance(V[u], V[v]);
        file << u << " " << v << " " << w << "\n";
        used.insert({u, v});
        written++;
    }

}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8"); 
    ofstream file("input.txt");
    int numOfVertices;
    int numOfTests = 100;
    double dense; 
    // cout << "\nКоличество тестов: ";
    // cin >> numOfTests;
    cout << "\nКоличество вершин : ";
    cin >> numOfVertices;
    int k; 
    cout << "Плотность графа (0.1 - 1.0)";
    cin >> dense;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> vertexPosition(0, 2.5 * sqrt(numOfVertices));
    uniform_int_distribution<> pickVertex(0, numOfVertices - 1);
    file << numOfTests << "\n";

    for (int testNum = 0; testNum < numOfTests; testNum++) {

        // Вершины
        vector<Point> V;
        set<pair<int,int>> used;
        while (V.size() < numOfVertices) {
            Point v;
            int x = vertexPosition(gen);
            int y = vertexPosition(gen);
            auto key = make_pair(x, y);
            if (used.count(key)) continue;  
            used.insert(key);
            V.push_back({x, y});
        }
        int start = pickVertex(gen);
        int end;
        do {
            end = pickVertex(gen);
        } while (end == start);
        // Запись в input.txt
        file << numOfVertices << " "
             << start << " "
             << end   << "\n";
        // Количество ребер для нужной плотности
        long long Emax = (long long)numOfVertices * (numOfVertices - 1) / 2;
        long long Etarget = (long long)floor(dense * (double)Emax);
        long long E = max(Etarget, (long long)(numOfVertices - 1)); // чтобы скелет связности влез
        if (E > Emax) E = Emax;
        file << E << "\n";
        // Вывод вершин
        for (auto& v : V) { 
            file << v.x << " " << v.y << "\n";
        }
        // Вывод рёбер
        generateDenseEdges(V, E, gen, file);
    }
    file.close();
    cout << "\nСгенерировано " << numOfTests << " тестов в input.txt\n";
    system("pause");
    return 0;
}
