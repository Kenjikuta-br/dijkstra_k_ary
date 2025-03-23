#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

class Graph {
public:
    std::vector<std::vector<std::pair<int, int>>> adj; // Lista de adjacência (pares com os vértices e pesos das arestas correspondentes)

    // Construtor que inicializa o grafo com N vértices
    Graph(int N) {
        adj.resize(N);     // Cria um vetor de adjacências de tamanho N
    }

    // Adiciona uma aresta ao grafo direcionado
    void addEdge(int u, int v, int weight) {
        adj[u].push_back(std::make_pair(v, weight));
    }

    // Retorna os vizinhos de um nó
    const std::vector<std::pair<int, int>>& getNeighbors(int node) const {
        return adj[node];
    }
};

#endif // GRAPH_HPP