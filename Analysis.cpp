#include <iostream>
#include <vector>
#include <tuple>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <queue>
#include <functional>  

const int INF = 2147483647; 

using namespace std;

struct Point {
    int x, y;
};

struct Edge {
    int from;
    int to;
    double cost;
};

struct TestResult{
    int dijkstra_time;
    int dijkstra_cover;
    int aStar_time;
    int aStar_cover;
};

vector<vector<int>> ribsToAdjacencyMatrix(int numVertices, const vector<Edge>& ribs) {
    vector<vector<int>> matrix(numVertices, vector<int>(numVertices, 0)); 
    for (int i = 0; i < numVertices; i++) {
        matrix[i][i] = 0;  // расстояние до самой себя = 0
    }
    // Заполнение матрицы смежности рёбрами
    for (const auto& rib : ribs) {
        matrix[rib.from][rib.to] = rib.cost;
        // matrix[rib.to][rib.from] = rib.cost;  // если граф неориентированный
    }
    return matrix;
}

double euclideanHeuristic(const Point& a, const Point& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void dijkstra(int n, const vector<vector<int>>& graph, int start, int end) {
    vector<int> best_distances(n, INF);  // best_distances[v] — лучшая известная дистанция от start до v
    vector<bool> visited(n, false);  // used[v] — уже найдено самое кратчайшее расстояние до v
    
    best_distances[start] = 0;

    for (int i = 0; i < n; i++) {
        // Ищем ближайшую не посещённую вершину
        int nearest = -1;
        for(int j = 0; j < n; j++){
            if( !visited[j] && 
                (nearest == -1 || best_distances[nearest] > best_distances[j])){
                nearest = j;
            }
        }
        
        if (best_distances[nearest] == INF){
            return;  // Если остались только недостижимые вершины
        } 
        
        if (nearest == end) {
            return; // Если дошли до целевой вершины
        }

        visited[nearest] = true;
        
        // Релаксация рёбер из nearest
        for (int v = 0; v < n; v++) {
            // Между nearest и v есть ребро
            if (graph[nearest][v] != 0 && !visited[v]) {
                int new_distance = best_distances[nearest] + graph[nearest][v]; // Cчитаем новый возможный путь
                if (new_distance < best_distances[v]) {
                    best_distances[v] = new_distance;
                }
            }
        }
    }
}

void aStar(int n, const vector<vector<int>>& graph, const vector<Point>& vertices, int start, int end){
    // open — приоритетная очередь, список вершин, до которых мы уже дошли
    // храним пары (f, v), где
    // f = g[v] + h(v) — оценка пути через вершину v
    // всегда достаём вершину с минимальным f
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> open;

    vector<int> g(n, INF);            // g[v] — стоимость пути от start до v
    vector<int> f(n, INF);            // f[v] = g[v] + h(v)
    vector<bool> closed(n, false);    // closed[v] — вершина "закрыта" (обработана)

    g[start] = 0;
    f[start] = euclideanHeuristic(vertices[start], vertices[end]);
    open.push({f[start], start});

    while (!open.empty()) {
        // Берём вершину с минимальным f
        int current = open.top().second;
        int curF = open.top().first;
        open.pop();
        if (closed[current]) continue; // Если вершина уже обработана — пропускаем
        if (curF > f[current]) continue; //Если уже смотрели всех её соседей
        if (current == end) return;  // Если дошли до целевой вершины

        closed[current] = true; // мы рассмотрели эту вершину и ВСЕ рёбра из неё
        // Рассматриваем всех соседей вершины current   
        for (int neighbor = 0; neighbor < n; neighbor++) {
            if (graph[current][neighbor] == 0) continue;   // Если между current и neighbor нет ребра — пропускаем
            if (closed[neighbor]) continue;                // уже обработан

            int new_g = g[current] + graph[current][neighbor]; // Считаем стоимость пути через current к neighbor

            // Релаксация рёбер (аналогично Дейкстре)
            if (new_g < g[neighbor]) {
                g[neighbor] = new_g;
                int h = euclideanHeuristic(vertices[neighbor], vertices[end]); // Эвристическая оценка расстояния от neighbor до end
                f[neighbor] = g[neighbor] + h; // Полная оценка пути через neighbor
                open.push({f[neighbor], neighbor}); // Добавляем вершину в очередь на обработку
            }
        }
    }
}


TestResult solveTestCase(ifstream& input) {
    TestResult result;
    int numOfVertices, start, end, numOfRibs ;
    input >> numOfVertices >> start >> end >> numOfRibs ;
    // Получает все вершины
    vector<Point> vertices;
    for (int i = 0; i < numOfVertices; i++) {
        Point p;
        input >> p.x >> p.y;
        vertices.push_back(p);
    }
    //Получает все ребра
    vector<Edge> ribs;
    for (int i = 0; i < numOfRibs; i++) {
        int from, to;
        double cost;
        Edge rib;
        input >> rib.from >> rib.to >> rib.cost;
        ribs.push_back(rib);
    }

    vector<vector<int>> graph = ribsToAdjacencyMatrix(numOfVertices, ribs);

    auto startTimeDijkstra = chrono::high_resolution_clock::now();
    dijkstra(numOfVertices, graph, start, end);
    auto endTimeDijkstra = chrono::high_resolution_clock::now();
    auto durationDijkstra = chrono::duration_cast<chrono::microseconds>(endTimeDijkstra - startTimeDijkstra);
    result.dijkstra_time = durationDijkstra.count();

    auto startTimeAStar = chrono::high_resolution_clock::now();
    aStar(numOfVertices, graph, vertices, start, end);
    auto endTimeAStar = chrono::high_resolution_clock::now();
    auto durationAStar = chrono::duration_cast<chrono::microseconds>(endTimeAStar - startTimeAStar);
    result.aStar_time = durationAStar.count();
    return result;
}


int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8"); 
    std::ifstream input("input.txt"); 
    std::ofstream output("results.txt");  

    double totalDijkstraTime = 0;
    double totalAStarTime = 0;
    int numOfTestCases;
    input >> numOfTestCases;
    
    TestResult result;
    vector<TestResult> results;
    for(int i = 0; i < numOfTestCases; i++){
        result = solveTestCase(input);
        totalDijkstraTime += result.dijkstra_time;
        totalAStarTime += result.aStar_time;
        results.push_back(result);
    }
    
    double averageDijkstraTime = totalDijkstraTime / numOfTestCases;
    double averageAStarTime = totalAStarTime / numOfTestCases;
    
    output << "Количество тестов: " << numOfTestCases << endl << endl;
    output << "Общее время выполнения алгоритма Дейкстры: " << totalDijkstraTime << " мкс" << endl;
    output << "Среднее время выполнения алгоритма Дейкстры: " << averageDijkstraTime << " мкс" << endl;
    output << "---" << endl;
    output << "Общее время выполнения алгоритма А*: " << totalAStarTime << " мкс" << endl;
    output << "Среднее время выполнения алгоритма А*: " << averageAStarTime << " мкс" << endl;
    output << "\n========================================\n\n";
    
    for(int i = 0; i < numOfTestCases; i++){
        output << "Тест " << i+1 << ": " << endl;
        output << "  Время работы алгоритма Дейкстры: " << results[i].dijkstra_time << " мкс" << endl;
        output << "  Время работы алгоритма А*: " << results[i].aStar_time << " мкс" << endl;
        output << "---" << endl;
    }
    
    input.close();
    output.close();
    cout << "The results are written to results.txt"<< endl;
    system("pause");
    return 0;
}