#include <iostream>
#include <queue>

class HeapNode {
public:
    int node_id;
    int priority;

    HeapNode(int id, int prio) : node_id(id), priority(prio) {}

    // Sobrecarga do operador < para ordenar pelo menor valor de prioridade
    bool operator<(const HeapNode& other) const {
        return priority < other.priority;
    }
};

int main() {
    // Criando objetos HeapNode
    HeapNode node1(1, 30);
    HeapNode node2(2, 10);
    HeapNode node3(3, 20);

    // Comparando os objetos usando a sobrecarga do operador <
    std::cout << (node1 < node2) << std::endl; // False porque 30 > 10
    std::cout << (node1 < node3) << std::endl; // True porque 30 > 20
    std::cout << (node2 < node3) << std::endl; // False porque 10 > 20

    return 0;
}