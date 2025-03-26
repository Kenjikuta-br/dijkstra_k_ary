#ifndef K_ARY_HEAP_HPP
#define K_ARY_HEAP_HPP

#include <vector>
#include "heap_node.hpp"

class KaryHeap {
private:
    std::vector<HeapNode> heap;  // Vetor de nós do heap
    int k;  // Grau do heap (número máximo de filhos por nó)
    std::vector<int> indexMap;   // Mapeia os índices dos nós no heap

    // Funções auxiliares para reorganizar o heap

    void heapifyUp(int idx);
    void heapifyDown(int idx);

public:
    // Construtor que inicializa a heap com grau k e número de vértices
    KaryHeap(int k, int numVertices);

    // Retorna o tamanho do heap
    int size() const;

    // Verifica se o heap está vazio
    bool isEmpty() const;

    // Insere um nó no heap
    void insert(const HeapNode& node);

    // Remove e retorna o nó de menor prioridade
    HeapNode extractMin();

    // Diminui a chave de um nó
    void decreaseKey(int node_id, int new_priority);

    // Retorna o nó de menor prioridade sem removê-lo
    const HeapNode& getMin() const;

    // Retorna a prioridade do vértice v
    int getPriority(int v) const;

    // Calcula a média dos r
    long double calculateAverageR() const;

    int getInsertCount() const { return insertCount; }
    int getExtractMinCount() const { return extractMinCount; }
    int getDecreaseKeyCount() const { return decreaseKeyCount; }

    //__________________________________________________________________________
    
    //          VARIÁVEIS
    //__________________________________________________________________________

    // Variáveis para contar os swaps
    int siftUpCount = 0;   // Contador para o número de swaps no heapifyUp
    int siftDownCount = 0; // Contador para o número de swaps no heapifyDown

    // Contadores para as operações
    int insertCount = 0;  // Contador de inserts
    int decreaseKeyCount = 0; // Contador de decreaseKey
    int extractMinCount = 0; // Contador de extractMin

    // Vetor para armazenar r (sift/ log_k n)
    std::vector<double> rValues;
};

#endif // K_ARY_HEAP_HPP
