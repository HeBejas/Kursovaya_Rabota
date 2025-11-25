#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <stdlib.h>
#include <fstream>

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
void solveTestCase(int index, ifstream& input, ofstream& output){
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
    vector<vector<double>> graph = ribsToAdjacencyMatrix(numOfRibs, ribs);
    vector<double> distances = dijkstra(numOfRibs, graph, start);

    output << "Тест " << index << ": ";
    if (distances[end] == INF) {
        output << "n/a" << endl;
    } else {
        output << distances[end] << endl;
    }
}
int main() {
    setlocale(LC_ALL, "Russian");

    std::ifstream input("input.txt"); 
    std::ofstream output("results.txt");  

    int numOfTestCases;
    input >> numOfTestCases;
    for(int i = 0; i < numOfTestCases; i++){
        solveTestCase(i+1,input,output);
    }
    input.close();
    output.close();
    cout << "The results are written to results.txt"<< endl;
    system("pause");
}    
// printMatrix(graph);