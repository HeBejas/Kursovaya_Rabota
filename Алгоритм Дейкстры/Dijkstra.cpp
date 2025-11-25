#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

const double INF = numeric_limits<double>::infinity();

vector<double> dijkstra(int n, const vector<vector<double>>& graph, int source) {
    vector<double> dist(n, INF); 
    vector<bool> visited(n, false); 
    dist[source] = 0;
    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j; 
            }
        }
        
        if (dist[u] == INF) break;
        
        visited[u] = true;
        
        for (int z = 0; z < n; z++) {
            if (graph[u][z] != INF && !visited[z]) {
                double newDist = dist[u] + graph[u][z];
                if (newDist < dist[z]) {
                    dist[z] = newDist;
                }
            }
        }
    }
    return dist; 
}


vector<vector<double>> ribsToAdjacencyMatrix(int numVertices, const vector<tuple<int, int, double>>& ribs){
    vector<vector<double>> matrix(numVertices, vector<double>(numVertices, INF));
     for (int i = 0; i < numVertices; i++) {
        matrix[i][i] = 0;
    }
    
    for (const auto& rib : ribs) {    
        int from = get<0>(rib);
        int to = get<1>(rib);
        double weight = get<2>(rib);
        
        matrix[from][to] = weight;
    }
    return matrix;
}

void printMatrix(const vector<vector<double>>& matrix) {
    cout << "Матрица смежности:" << endl;
    for (const auto& row : matrix) {
        for (double weight : row) {
            cout << weight << " ";
        }
        cout << endl;
    }
}

struct TestResult{
    int distance;
    int time;
    string text;
};

TestResult solveTestCase(ifstream& input){
    TestResult result;

    int numOfRibs;
    int start;
    int end;
    input >> numOfRibs >> start >> end;
    vector<tuple<int, int, double>> ribs(numOfRibs);
    for(int i = 0; i < numOfRibs; i++){
        int u, v, w;
        input >> u >> v >> w;
        ribs[i] = make_tuple(u, v, w);
    }

    auto startTime = chrono::high_resolution_clock::now();

    vector<vector<double>> graph = ribsToAdjacencyMatrix(numOfRibs, ribs);
    vector<double> distances = dijkstra(numOfRibs, graph, start);

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    
    result.time = duration.count(); 

    if (distances[end] == INF) {
        result.text = "Путь не найден";
        result.distance = -1;
    } else {
        result.text = "Путь найден";
        result.distance = distances[end];
    }
    return result;
}
int main() {
    std::ifstream input("input.txt"); 
    std::ofstream output("results.txt");  

    output << setw(150);
    output << "\n";
    output << endl << endl << endl << endl << endl;
    int numOfTestCases;
    input >> numOfTestCases;
    double totalTime = 0;
    for(int i = 0; i < numOfTestCases; i++){
        TestResult result = solveTestCase(input);
        output << "Тест " << i+1 << ": " << endl;
        output << "  Статус: " << result.text << endl;
        output << "  Результат: " << result.distance << endl;
        output << "  Время: " << result.time << " мкс" << endl;
        output << "---" << endl;
        totalTime += result.time;
    }
    double averageTime = totalTime / numOfTestCases;
    output.seekp(0);
    output << "Общее время выполнения: " << totalTime << " мкс" << endl;
    output << "Среднее время выполнения: " << averageTime << " mks" << endl;
    output << "Количество тестов: " << numOfTestCases << endl << endl;
    input.close();
    output.close();
    cout << "The results are written to results.txt"<< endl;
    system("pause");
}    
// printMatrix(graph);