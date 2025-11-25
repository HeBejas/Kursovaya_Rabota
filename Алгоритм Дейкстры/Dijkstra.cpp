#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <stdlib.h>

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
                    // Здесь можно добавить: prev[v] = u; для сохранения пути
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
    
int main() {
    setlocale(LC_ALL, "Russian");
    int n;
    int start;
    int end;
    vector<tuple<int, int, double>> ribs(n);
    cin >> n;
    cin >> start;
    cin >> end;
    for(int i = 0; i < n; i++){
        int u, v, w;
        cin >> u >> v >> w;
        ribs[i] = make_tuple(u, v, w);
    }

    vector<vector<double>> graph = ribsToAdjacencyMatrix(n, ribs);
    vector<double> distances = dijkstra(n, graph, start);
    
    cout << "\nКратчайшее расстояние до вершины " << end << ": ";
    if (distances[end] == INF) {
        cout << "n/a" << endl;
    } else {
        cout << distances[end] << endl;
    }
    system("pause");
    printMatrix(graph);
}