#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <queue>
#include <functional>  

using namespace std;
const int INF = 1000000000;

struct Point {
    int x, y;
};

struct TestResult{
    int dijkstraTime;
    int aStarTime;
};

vector<vector<int>> ribsToAdjacencyMatrix(int n, const vector<tuple<int, int, int>>& ribs){
    vector<vector<int>> matrix(n, vector<int>(n, INF));
    
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;
    }
    
    for (const auto& rib : ribs) {    
        int from = get<0>(rib);
        int to = get<1>(rib);
        int weight = get<2>(rib);
        matrix[from][to] = weight;
    }
    
    return matrix;
}

double euclideanHeuristic(const Point& a, const Point& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void dijkstra(int n, const vector<vector<int>>& graph, int start, int end) {
    vector<int> dist(n, INF); 
    vector<bool> visited(n, false); 
    dist[start] = 0;
    
    for (int i = 0; i < n; i++) {
        int nearest = -1;
        
        for(int j = 0; j < n; j++){
            if(!visited[j] && (nearest == -1 || dist[nearest] > dist[j])){
                nearest = j;
            }
        }

        if (nearest == -1 || dist[nearest] == INF) break; 
        visited[nearest] = true;

        if (nearest == end) return;
        
        for (int z = 0; z < n; z++) { 
            if (graph[nearest][z] != INF && !visited[z]) { 
                int newDist = dist[nearest] + graph[nearest][z]; 
                if (newDist < dist[z]) {  
                    dist[z] = newDist;  
                }
            }
        }

    }
}

void aStar(int n, const vector<vector<int>>& graph, const vector<Point>& coordinates, int start, int end) {
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> openList;
    vector<bool> closedList(n, false);
    vector<double> g(n, INF);
    vector<double> f(n, INF);
    
    g[start] = 0;
    f[start] = euclideanHeuristic(coordinates[start], coordinates[end]);
    openList.push({f[start], start});
    while (!openList.empty()) {
        int current = openList.top().second;
        openList.pop();
        
        if (closedList[current]) continue;
        
        if (current == end) return;
        
        closedList[current] = true;
        
        for (int neighbor = 0; neighbor < n; neighbor++) {
            if (graph[current][neighbor] == INF || closedList[neighbor]) continue;
            
            double new_g = g[current] + graph[current][neighbor];

            if (new_g < g[neighbor]) {
                g[neighbor] = new_g;
                double h = euclideanHeuristic(coordinates[neighbor], coordinates[end]);
                f[neighbor] = g[neighbor] + h;
                openList.push({f[neighbor], neighbor});
            }
        }
    }
}


TestResult solveTestCase(ifstream& input) {
    TestResult result;

    int numVertices, numRibs, start, end;
    input >> numVertices >> numRibs >> start >> end;
    vector<Point> coordinates;

    for (int i = 0; i < numVertices; i++) {
        Point p;
        input >> p.x >> p.y;
        coordinates.push_back(p);
    }
    vector<tuple<int, int, int>> ribs;
    ribs.reserve(numRibs);

    for (int i = 0; i < numRibs; i++) {
        int u, v, w;
        input >> u >> v >> w;
        ribs.push_back(make_tuple(u, v, w));
    }

    vector<vector<int>> graph = ribsToAdjacencyMatrix(numVertices, ribs);

    auto startTimeDijkstra = chrono::high_resolution_clock::now();
    dijkstra(numVertices, graph, start, end);
    auto endTimeDijkstra = chrono::high_resolution_clock::now();
    auto durationDijkstra = chrono::duration_cast<chrono::microseconds>(endTimeDijkstra - startTimeDijkstra);
    result.dijkstraTime = durationDijkstra.count();

    auto startTimeAStar = chrono::high_resolution_clock::now();
    aStar(numVertices, graph, coordinates, start, end);
    auto endTimeAStar = chrono::high_resolution_clock::now();
    auto durationAStar = chrono::duration_cast<chrono::microseconds>(endTimeAStar - startTimeAStar);
    result.aStarTime = durationAStar.count();

    return result;
}


int main() {
    std::ifstream input("input.txt"); 
    std::ofstream output("results.txt");  

    double totalDijkstraTime = 0;
    double totalAStarTime = 0;
    int numOfTestCases;
    input >> numOfTestCases;
    
    vector<TestResult> results;
    results.reserve(numOfTestCases);
    
    for(int i = 0; i < numOfTestCases; i++){
        results.push_back(solveTestCase(input));
        totalDijkstraTime += results.back().dijkstraTime;
        totalAStarTime += results.back().aStarTime;
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
        output << "  Время работы алгоритма Дейкстры: " << results[i].dijkstraTime << " мкс" << endl;
        output << "  Время работы алгоритма А*: " << results[i].aStarTime << " мкс" << endl;
        output << "---" << endl;
    }
    
    input.close();
    output.close();
    cout << "The results are written to results.txt"<< endl;
    system("pause");
    return 0;
}