#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


std::vector<std::vector<bool>> createGraph(const std::vector<std::vector<int>>& hyperedges, int numVertices)
{
    int numRows = hyperedges.size();
  
    std::vector<std::vector<bool>> graph(numVertices, std::vector<bool>(numVertices, false));
    
    // Wypełnij macierz sąsiedztwa na podstawie możliwych kombinacji krawędzi w hiperkrawędzi 
    for (const auto& row : hyperedges) {
        for (int i = 0; i < row.size(); ++i) {
            for (int j = i + 1; j < row.size(); ++j) {
                graph[row[i]][row[j]] = true;
                graph[row[j]][row[i]] = true;
            }
        }
    }

    return graph;
}

// Funkcja do generowania losowego hipergrafu zależnego od liczby wierzchołków
std::vector<std::vector<int>> generateRandomHypergraph(int numVertices, int maxHyperedges) {
    std::vector<std::vector<int>> hyperedges;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> numHyperedgesDistribution(1, maxHyperedges);
    std::uniform_int_distribution<> verticesDistribution(0, numVertices - 1);

    int numHyperedges = numHyperedgesDistribution(gen);

    for (int i = 0; i < numHyperedges; ++i) {
        std::vector<int> hyperedge;
        int numVerticesInHyperedge = verticesDistribution(gen) + 1; // Dodaj 1, aby uniknąć hipergrafu bez wierzchołków

        for (int j = 0; j < numVerticesInHyperedge; ++j) {
            int vertex = verticesDistribution(gen);

            //Warunek, aby wierzchołek nie był połączony krawędzią sam ze sobą
            while (std::find(hyperedge.begin(), hyperedge.end(), vertex) != hyperedge.end()) {
                vertex = verticesDistribution(gen);
            }

            hyperedge.push_back(vertex);
        }

        hyperedges.push_back(hyperedge);
    }

    return hyperedges;
}

// Funkcja do zapisywania hipergrafu do pliku JSON
void saveHypergraphToFile(const std::vector<std::vector<int>>& hypergraph, const std::string& filename) {
    json jsonData;
    jsonData["hyperedges"] = hypergraph;

    std::ofstream output(filename);
    if (output.is_open()) {
        output << jsonData.dump(4); // Dump JSON with indentation of 4 spaces
        std::cout << "Hypergraph saved to: " << filename << '\n';
    }
    else {
        std::cerr << "Error opening output file." << '\n';
    }
}

// Funkcja do wyświetlania kolorowania
void printColoring(const std::vector<int>& colors) {
    for (int color : colors) {
        std::cout << color << " ";
    }
    std::cout << '\n';
}

// Funkcja implementująca algorytm zachłanny kolorowania wierzchołków grafu
std::vector<int> greedyColoring(const std::vector<std::vector<bool>>& graph) {
    int numVertices = graph.size();
    std::vector<int> colors(numVertices, 0);

    colors[0] = 1;

    for (int i = 1; i < numVertices; i++) {
        for (int k = 1; k <= numVertices; k++) {
            bool goodColor = true;

            for (int j = 0; j < i; j++) {
                if (graph[i][j] && colors[j] == k) {
                    goodColor = false;
                    break;
                }
            }

            if (goodColor) {
                colors[i] = k;
                break;
            }
        }
    }

    return colors;
}

// Funkcja implementująca algorytm zachłanny z losowym wyborem kolejności wierzchołków kolorowania grafu
std::vector<int> randomGreedyColoring(const std::vector<std::vector<bool>>& graph) {
    int numVertices = graph.size();
    std::vector<int> colors(numVertices, 0);
    std::vector<int> permutation(numVertices);

    for (int i = 0; i < numVertices; i++) {
        permutation[i] = i;
    }

    srand(time(0));
    std::random_shuffle(permutation.begin(), permutation.end());

    colors[permutation[0]] = 1;

    for (int i = 1; i < numVertices; i++) {
        for (int k = 1; k <= numVertices; k++) {
            bool goodColor = true;

            for (int j = 0; j < i; j++) {
                if (graph[permutation[i]][permutation[j]] && colors[permutation[j]] == k) {
                    goodColor = false;
                    break;
                }
            }

            if (goodColor) {
                colors[permutation[i]] = k;
                break;
            }
        }
    }

    return colors;
}



//Brute Force Coloring
bool isSafeToColor(std::vector<std::vector<bool>>& graph, std::vector<int>& color, int vertex, int c) {
    int numVertices = graph.size();
   
    for (int i = 0; i < numVertices; i++) {
        if (graph[vertex][i] == 1 && color[i] == c) {
            return false;
        }
    }
    return true;
}
//Rekurencyjna funkcja przeglądająca wszystkie możliwe rozwiązania
bool graphColoringUtil(std::vector<std::vector<bool>>& graph, int numColors, int vertex, std::vector<int>& color) {
    int numVertices = graph.size();
  
    if (vertex == numVertices) {
        if (isSafeToColor(graph, color, vertex - 1, color[vertex - 1])) {
            return true;
        }
        return false;
    }

    for (int c = 1; c <= numColors; c++) {
        if (isSafeToColor(graph, color, vertex, c)) {
            color[vertex] = c;
            if (graphColoringUtil(graph, numColors, vertex + 1, color)) {
                return true;
            }
            color[vertex] = 0; 
        }
    }

    return false;
}

std::vector<int> bruteForceColoring(std::vector<std::vector<bool>>& graph) {
    int numVertices = graph.size();
    int numColors = numVertices;
    std::vector<int> color(numVertices, 0); // Initialize colors with 0

    if (!graphColoringUtil(graph, numColors, 0, color)) {
        std::cout << "Solution does not exist." << '\n';
    }
    return color;
}


int main() {
    
    // Ustawienia generatora
    int n = 20; // Dowolna liczba wierzchołków
    int maxHyperedges = 5; // Maksymalna liczba hiperkrawędzi

    // Generuj losowy hipergraf
    std::vector<std::vector<int>> randomHypergraph = generateRandomHypergraph(n, maxHyperedges);

    // Zapisz hipergraf do pliku JSON
    saveHypergraphToFile(randomHypergraph, "random_hypergraph.json");
   
    //Obsługa pliku
    std::ifstream input("hypergraph.json");                  //random_hypergraph.json
    if (!input.is_open()) {                                         //hypergraph.json
        std::cerr << "Error opening input file." << '\n';           //hypergraph2.json
        return 1;
    }

    json inputData;
    input >> inputData;
    input.close();

    
    //Tworzenie macierzy z hiperkrawędziami
    std::vector<std::vector<int>> hyperedges;
    for (const auto& hyperedge : inputData["hyperedges"]) {
        std::vector<int> vertices;
        for (int vertex : hyperedge) {
            vertices.push_back(vertex);
        }
        hyperedges.push_back(vertices);
    }
   
    //Zliczanie liczby wierzchołków
    int numVertices = 0; // Liczba wierzchołków
    for (const auto& hyperedge : hyperedges) {
        numVertices = std::max(numVertices, *std::max_element(hyperedge.begin(), hyperedge.end()));
    }
    ++numVertices; 

    //Tworzenie grafu przy pomocy macierzy sąsiedztwa reprezentującego hipergraf
    std::vector<std::vector<bool>> graph = createGraph(hyperedges, numVertices);
   

    //Algorytm pełnego przeglądu możliwych rozwiązań
    std::vector<int>bruteColoring = bruteForceColoring(graph);
    std::cout << "Brute Force Algorithm: ";
    printColoring(bruteColoring);
   
     // Algorytm zachłanny
    std::vector<int> greedyColors = greedyColoring(graph);
    std::cout << "Greedy Algorithm: ";
    printColoring(greedyColors);

    // Algorytm zachłanny z losowym wyborem kolejności wierzchołków
    std::vector<int> randomGreedyColors = randomGreedyColoring(graph);
    std::cout << "Random Greedy Algorithm: ";
    printColoring(randomGreedyColors);


    return 0;
}