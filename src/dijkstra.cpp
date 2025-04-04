#include "dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <omp.h>

// Armazena a referência do grafo.
// Cria um vetor para armazenar as menores distâncias, inicialmente definidas como infinito.
Dijkstra::Dijkstra(const Graph& graph) : graph(graph) {
    distances.resize(graph.adj.size(), std::numeric_limits<int>::max());
}

void Dijkstra::computeShortestPaths(int source, int k, bool kenjiFlag) {
    // Garantindo que está utilizando apenas uma thread
    // omp_set_num_threads(1); 

    // Use nanoseconds ou microseconds em vez de milliseconds
    auto start_time = std::chrono::high_resolution_clock::now();

    int n = graph.adj.size();
    distances.assign(n, std::numeric_limits<int>::max());  // Resetar distâncias
    distances[source] = 0;

    KaryHeap heap(k, n);
    std::vector<bool> inHeap(n, false);  // Para acompanhar quais nós estão na heap

    // Insere o nó de origem diretamente
    heap.insert(HeapNode(source, 0));
    inHeap[source] = true;

    // Processa os nós
    while (!heap.isEmpty()) {
        HeapNode currentNode = heap.extractMin();
        int u = currentNode.node_id;
        inHeap[u] = false;

        for (const auto& neighbor : graph.getNeighbors(u)) {
            int v = neighbor.first;
            int weight = neighbor.second;

            int newDist = distances[u] + weight;
            if (newDist < distances[v]) {
                distances[v] = newDist;

                // Se o nó v não estiver na heap, insira-o
                if (!inHeap[v]) {
                    heap.insert(HeapNode(v, newDist));
                    inHeap[v] = true;
                } else {
                    // Caso contrário, apenas diminua a chave se o nó já estiver na heap
                    heap.decreaseKey(v, newDist);
                }
            }
        }
    }

    // Medição do tempo
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    if(kenjiFlag){
        std::cout << graph.getNumEdges() << ";"
          << heap.getInsertCount() << ";"
          << heap.getExtractMinCount() << ";"
          << heap.getDecreaseKeyCount() << ";"
          << heap.calculateAverageInsertR() << ";"
          << heap.calculateAverageExtractMinR() << ";"
          << heap.calculateAverageDecreaseKeyR() << ";"
          << duration.count() << ";";
    } else {
        std::cout << "Número de arestas: " << graph.getNumEdges() << std::endl;
        std::cout << "Número de inserts: " << heap.getInsertCount() << std::endl;
        std::cout << "Número de extractMin: " << heap.getExtractMinCount() << std::endl;
        std::cout << "Número de decreaseKey: " << heap.getDecreaseKeyCount() << std::endl;
        std::cout << "Média de r para inserts: " << heap.calculateAverageInsertR() << std::endl;
        std::cout << "Média de r para extractMin: " << heap.calculateAverageExtractMinR() << std::endl;
        std::cout << "Média de r para decreaseKey: " << heap.calculateAverageDecreaseKeyR() << std::endl;
        std::cout << "Tempo de execução: " << duration.count() << " µs" << std::endl;
    }
}

int Dijkstra::getDistance(int node) const {
    return distances[node];
}

std::vector<int> Dijkstra::getAllDistances() const {
    return distances;
}