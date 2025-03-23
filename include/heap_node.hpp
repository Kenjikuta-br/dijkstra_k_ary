#ifndef HEAP_NODE_HPP
#define HEAP_NODE_HPP

class HeapNode {
public:
    int node_id;   // Identificação do nó no grafo, cada node_id correspode a um vértice
    int priority;  // Distância mínima no Dijkstra que irá ir diminuindo, startada como infinito

    // Construtor
    HeapNode(int id, int prio) : node_id(id), priority(prio) {}

    // Sobrecarga do operador < para ordenar pelo menor valor de prioridade
    bool operator<(const HeapNode& other) const {
        return priority < other.priority;
    }
};

#endif // HEAP_NODE_HPP
