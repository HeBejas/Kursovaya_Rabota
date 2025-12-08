#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

int main() {
    ofstream file("input.txt");
    int numOfTests, numOfVertices, numOfRibs;
    cout << "\nNum of tests : ";
    cin >> numOfTests;
    cout << "\nNum of Vertices : ";
    cin >> numOfVertices;
    cout << "\nNum of Ribs : ";
    cin >> numOfRibs;
    const int minWeight = 1;
    const int maxWeight = 100;
    const int coordRange = 1000;

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> weightDist(minWeight, maxWeight);
    uniform_int_distribution<> coordDist(0, coordRange - 1);

    file << numOfTests << "\n";

    for (int testNum = 0; testNum < numOfTests; testNum++) {
        uniform_int_distribution<> vertexDist(0, numOfVertices - 1);

        int start = vertexDist(gen);
        int end = vertexDist(gen);
        while (start == end) end = vertexDist(gen);

        file << numOfVertices << " " << numOfRibs << " " << start << " " << end << "\n";

        for (int i = 0; i < numOfVertices; ++i) {
            int x, y;
            do {
                x = coordDist(gen);
                y = coordDist(gen);
            } while (x == y);
            file << x << " " << y << "\n";
        }

        // Генерация УНИКАЛЬНЫХ ребер
        vector<vector<bool>> edgeExists(numOfVertices, vector<bool>(numOfVertices, false));
        int ribsGenerated = 0;
        
        // Сначала создаем остовное дерево для гарантии связности
        for (int i = 1; i < numOfVertices; i++) {
            int u = i;
            int v = vertexDist(gen) % i;  // Соединяем с ранее созданной вершиной
            int w = weightDist(gen);
            
            file << v << " " << u << " " << w << "\n";
            edgeExists[v][u] = true;
            edgeExists[v][u] = true; 
            ribsGenerated++;
        }
        
        // Генерация оставшихся ребер
        while (ribsGenerated < numOfRibs) {
            int u = vertexDist(gen);
            int v = vertexDist(gen);
            
            if (u == v || edgeExists[u][v]) continue;
            
            int w = weightDist(gen);
            file << u << " " << v << " " << w << "\n";
            edgeExists[u][v] = true;
            edgeExists[v][u] = true; 
            ribsGenerated++;
        }
        
    }

    file.close();
    cout << "\nGenerated " << numOfTests << " tests in input.txt\n";
    system("pause");
    return 0;
}