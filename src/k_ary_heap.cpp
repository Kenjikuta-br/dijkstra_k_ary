#include "k_ary_heap.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

// Construtor: Inicializa a heap e define o tamanho do indexMap
KaryHeap::KaryHeap(int k, int numVertices) : k(k), indexMap(numVertices, -1) {}

int KaryHeap::size() const {
    return heap.size();
}

bool KaryHeap::isEmpty() const {
    return heap.empty();
}

void KaryHeap::insert(const HeapNode& node) {
    if (indexMap[node.node_id] != -1) {
        throw std::invalid_argument("Node already exists in heap.");
    }

    // Insere o novo nó no final
    heap.push_back(node);
    int pos = heap.size() - 1;
    indexMap[node.node_id] = pos;

    // Reorganiza a heap para manter a propriedade
    heapifyUp(pos);
}

HeapNode KaryHeap::extractMin() {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty.");
    }

    // O menor nó é a raiz (primeiro elemento)
    HeapNode minNode = heap[0];

    // Move o último nó para a raiz
    heap[0] = heap.back();
    indexMap[heap[0].node_id] = 0;

    // Remove o último nó
    heap.pop_back();
    indexMap[minNode.node_id] = -1; // Remove a referência no indexMap

    // Reorganiza o heap
    if (!heap.empty()) {
        heapifyDown(0);
    }

    return minNode;
}

void KaryHeap::decreaseKey(int node_id, int new_priority) {
    int idx = indexMap[node_id];

    //std::cout << "node_id: " << node_id << " com prioridade " << new_priority << std::endl;

    if (idx == -1) {
        std::cout << "FUDEUUUU   node_id: " << node_id << " com prioridade " << new_priority << std::endl;
        throw std::invalid_argument("Node not found in heap.");
    }

    if (new_priority >= heap[idx].priority) {
        throw std::invalid_argument("New priority is not smaller than current priority.");
    }

    // Atualiza a prioridade do nó
    heap[idx].priority = new_priority;

    // Reorganiza a heap
    heapifyUp(idx);
}

const HeapNode& KaryHeap::getMin() const {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty.");
    }
    return heap[0];
}

void KaryHeap::heapifyUp(int idx) {
    int parentIdx = (idx - 1) / k;

    if (idx > 0 && (heap[idx].priority < heap[parentIdx].priority)) {
        std::swap(heap[idx], heap[parentIdx]);

        // Atualiza os índices no indexMap
        indexMap[heap[idx].node_id] = idx;
        indexMap[heap[parentIdx].node_id] = parentIdx;

        heapifyUp(parentIdx);
    }
}

void KaryHeap::heapifyDown(int idx) {
    int minIdx = idx;
    int firstChildIdx = this->k * idx + 1;
    long unsigned int k_provisory = static_cast<long unsigned int>(this->k);

    for (long unsigned int i = 0; i < k_provisory && firstChildIdx + i < heap.size(); ++i) {
        int childIdx = firstChildIdx + i;
        if (heap[childIdx].priority < heap[minIdx].priority) {
            minIdx = childIdx;
        }
    }

    if (minIdx != idx) {
        std::swap(heap[idx], heap[minIdx]);

        // Atualiza os índices no indexMap
        indexMap[heap[idx].node_id] = idx;
        indexMap[heap[minIdx].node_id] = minIdx;

        heapifyDown(minIdx);
    }
}

int KaryHeap::getPriority(int v) const {
    int idx = indexMap[v];
    
    // Verifica se o nó existe na heap
    if (idx == -1) {
        throw std::invalid_argument("Node not found in heap.");
    }

    // Retorna a prioridade (distância) do nó v
    return heap[idx].priority;
}


/*
int main() {
    // Vamos criar um heap ternário (k=3) com 5 nós
    KaryHeap heap(3, 5); // k = 3, numVertices = 5

    // Vamos inserir alguns nós com diferentes prioridades
    heap.insert(HeapNode(0, 10)); // Nó 0 com prioridade 10
    heap.insert(HeapNode(1, 20)); // Nó 1 com prioridade 20
    heap.insert(HeapNode(2, 5));  // Nó 2 com prioridade 5
    heap.insert(HeapNode(3, 30)); // Nó 3 com prioridade 30
    heap.insert(HeapNode(4, 15)); // Nó 4 com prioridade 15

    // O nó de menor prioridade deve ser o nó 2, com prioridade 5
    std::cout << "Min node before extraction: " << heap.getMin().node_id << " with priority " << heap.getMin().priority << std::endl;

    // Vamos extrair o nó de menor prioridade
    HeapNode minNode = heap.extractMin();
    std::cout << "Extracted min node: " << minNode.node_id << " with priority " << minNode.priority << std::endl;

    // Agora, o nó com menor prioridade será o nó 0, com prioridade 10
    std::cout << "Min node after extraction: " << heap.getMin().node_id << " with priority " << heap.getMin().priority << std::endl;

    // Vamos diminuir a chave do nó 4, de 15 para 3
    heap.decreaseKey(4, 3);
    std::cout << "Min node after decreasing key: " << heap.getMin().node_id << " with priority " << heap.getMin().priority << std::endl;

    // Extrair o novo nó de menor prioridade
    minNode = heap.extractMin();
    std::cout << "Extracted min node after decreasing key: " << minNode.node_id << " with priority " << minNode.priority << std::endl;

    return 0;
}

*/