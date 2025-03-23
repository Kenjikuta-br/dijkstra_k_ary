#include "dijkstra.hpp"
#include <iostream>
//Armazena a referência do grafo.
//Cria um vetor para armazenar as menores distâncias, inicialmente definidas como infinito.
Dijkstra::Dijkstra(const Graph& graph) : graph(graph) {
    distances.resize(graph.adj.size(), std::numeric_limits<int>::max());
}

void Dijkstra::computeShortestPaths(int source, int k) {
    int n = graph.adj.size();
    distances.assign(n, std::numeric_limits<int>::max());  // Resetar distâncias
    distances[source] = 0;

    KaryHeap heap(k, n);  // Heap ternário
    // Insere todos os nós com distância infinita, exceto o nó de origem
    for (int i = 0; i < n; ++i) {
        int distValue = (i == source) ? 0 : std::numeric_limits<int>::max();
        heap.insert(HeapNode(i, distValue));
    }
    // Processa os nós
    while (!heap.isEmpty()) {
        HeapNode currentNode = heap.extractMin();
        int u = currentNode.node_id;

        for (const auto& neighbor : graph.getNeighbors(u)) {
            int v = neighbor.first;
            int weight = neighbor.second;

            int newDist = distances[u] + weight;
            if (newDist < distances[v]) {
                distances[v] = newDist;
                heap.decreaseKey(v, newDist);
            }
        }
    }
}

int Dijkstra::getDistance(int node) const {
    return distances[node];
}

std::vector<int> Dijkstra::getAllDistances() const {
    return distances;
}


int main() {
    // Criando um grafo de exemplo
    Graph graph(5);
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 5);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 1, 3);
    graph.addEdge(2, 3, 9);
    graph.addEdge(3, 4, 2);

    // Criando uma instância de Dijkstra e rodando o algoritmo a partir do nó 0
    Dijkstra dijkstra(graph);
    dijkstra.computeShortestPaths(0, 4);

    // Exibindo as distâncias mínimas
    for (int i = 0; i < 5; ++i) {
        std::cout << "Distância de 0 até " << i << ": " << dijkstra.getDistance(i) << std::endl;
    }

    return 0;
}
