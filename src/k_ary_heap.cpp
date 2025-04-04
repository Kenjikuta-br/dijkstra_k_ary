#include "k_ary_heap.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cmath>

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

    // Incrementa o contador de inserts
    insertCount++;

    // Calcula r para a operação de insert
    double log_k_n = std::log(heap.size()) / std::log(k);
    double r = (log_k_n == 0) ? 0 : static_cast<double>(siftUpCount) / log_k_n;
    insertRValues.push_back(r);

    // Zera o contador de sift-ups após calcular r
    siftUpCount = 0;
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

    // Incrementa o contador de extractMin
    extractMinCount++;

    // Calcula r para a operação de extractMin
    double log_k_n = std::log(heap.size() + 1) / std::log(k); // +1 because we just removed an element
    double r = (log_k_n == 0) ? 0 : static_cast<double>(siftDownCount) / log_k_n;
    extractMinRValues.push_back(r);

    // Zera o contador de sift-downs após calcular r
    siftDownCount = 0;

    return minNode;
}

void KaryHeap::decreaseKey(int node_id, int new_priority) {
    int idx = indexMap[node_id];

    if (idx == -1) {
        throw std::invalid_argument("Node not found in heap.");
    }

    if (new_priority >= heap[idx].priority) {
        throw std::invalid_argument("New priority is not smaller than current priority.");
    }

    // Atualiza a prioridade do nó
    heap[idx].priority = new_priority;

    // Reorganiza a heap
    heapifyUp(idx);

    // Incrementa o contador de decreaseKey
    decreaseKeyCount++;

    // Calcula r para a operação de decreaseKey
    double log_k_n = std::log(heap.size()) / std::log(k);
    double r = (log_k_n == 0) ? 0 : static_cast<double>(siftUpCount) / log_k_n;
    decreaseKeyRValues.push_back(r);

    // Zera o contador de sift-ups após calcular r
    siftUpCount = 0;
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

        // Incrementa o contador de sift-up
        siftUpCount++;

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

        // Incrementa o contador de sift-down
        siftDownCount++;

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

long double KaryHeap::calculateAverageInsertR() const {
    if (insertRValues.empty()) {
        return 0.0L;
    }
    long double sum = 0.0L;
    for (long double r : insertRValues) {
        sum += r;
    }
    return sum / insertRValues.size();
}

long double KaryHeap::calculateAverageExtractMinR() const {
    if (extractMinRValues.empty()) {
        return 0.0L;
    }
    long double sum = 0.0L;
    for (long double r : extractMinRValues) {
        sum += r;
    }
    return sum / extractMinRValues.size();
}

long double KaryHeap::calculateAverageDecreaseKeyR() const {
    if (decreaseKeyRValues.empty()) {
        return 0.0L;
    }
    long double sum = 0.0L;
    for (long double r : decreaseKeyRValues) {
        sum += r;
    }
    return sum / decreaseKeyRValues.size();
}