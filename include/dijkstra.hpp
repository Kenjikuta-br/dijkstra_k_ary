#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph.hpp"
#include "k_ary_heap.hpp"
#include <vector>
#include <limits>

class Dijkstra {
public:
    // Construtor
    Dijkstra(const Graph& graph);

    // Calcula os caminhos mínimos a partir do nó de origem
    void computeShortestPaths(int source,  int k, bool kenjiFlag);

    // Retorna a distância mínima até um nó específico
    int getDistance(int node) const;

    // Retorna todas as distâncias mínimas
    std::vector<int> getAllDistances() const;

private:
    const Graph& graph;
    std::vector<int> distances;
};

#endif // DIJKSTRA_HPP
