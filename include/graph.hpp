#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>  // Add this line

class Graph {
public:
    std::vector<std::vector<std::pair<int, int>>> adj; // Lista de adjacência (pares com os vértices e pesos das arestas correspondentes)

    // Construtor default que cria um grafo vazio
    Graph() {
        adj.resize(0);  // Inicializa um grafo vazio
    }

    // Construtor que inicializa o grafo com N vértices
    Graph(int N) {
        adj.resize(N);     // Cria um vetor de adjacências de tamanho N
    }

    // Função resize para ajustar o número de vértices no grafo
    void resize(int N) {
        adj.resize(N);     // Ajusta o tamanho do vetor de adjacências para N vértices
    }

    // Adiciona uma aresta ao grafo direcionado
    void addEdge(int u, int v, int weight) {
        adj[u].push_back(std::make_pair(v, weight));
    }

    // Retorna os vizinhos de um nó
    const std::vector<std::pair<int, int>>& getNeighbors(int node) const {
        return adj[node];
    }

    void read_dimacs(std::istream& in);
};

#endif // GRAPH_HPP