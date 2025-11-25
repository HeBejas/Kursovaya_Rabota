#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <stdlib.h>

using namespace std;

int main() {
    ofstream file("input.txt");
    int numTests;
    cout << "\n Num of tests : \n";
    cin >> numTests;
    int minNumOfVertices = 10; 
    int maxNumOfVertices = 300; 
    int minNumOfRibs = 300; 
    int maxNumOfRibs = 500;
    int minWeight = 1; 
    int maxWeight = 100;  

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> genNumOfVertexes(minNumOfVertices, maxNumOfVertices);
    uniform_int_distribution<> genNumOfRibs(minNumOfRibs, maxNumOfRibs);
    uniform_int_distribution<> weightDist(minWeight, maxWeight);
    
    for (int testNum = 0; testNum < numTests; testNum++) {
        int numOfVertexes = genNumOfVertexes(gen);
        int numOfRibs = genNumOfRibs(gen);
        uniform_int_distribution<> vertexPoint(0, numOfVertexes - 1);

        int start = vertexPoint(gen);
        int end = vertexPoint(gen);
        while (start == end) end = vertexPoint(gen); 
        file << numOfRibs << " " << start << " " << end << "\n";
        while(numOfRibs--){
            int u = vertexPoint(gen); 
            int v = vertexPoint(gen);
            int w = weightDist(gen);  
            file << u << " " << v << " " << w << "\n";
        }
    }
    if (file.is_open()) {
        file.close();
        cout << "Generated " << numTests << " tests\n";
    } else {
        cerr << "Error\n";
        return 1;
    }
    system("pause");
    return 0;
}