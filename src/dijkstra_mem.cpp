#include "dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unistd.h>

// Function to measure memory usage
size_t memory_used(bool resident = false) {
    std::ifstream statm("/proc/self/statm");
    if (!statm) {
        std::cerr << "Error: Could not open /proc/self/statm" << std::endl;
        return 0;
    }

    size_t pages;
    if (resident) {
        statm >> pages >> pages;  // Read second value (RSS)
    } else {
        statm >> pages;  // Read first value (VM size)
    }
    
    return pages * sysconf(_SC_PAGESIZE);
}

void runDijkstraWithMemoryMeasurement(const std::string& filename) {
    std::cout << "Running Dijkstra on " << filename << std::endl;

    // Open file as an input stream
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Lê o grafo a partir do arquivo
    Graph graph;
    graph.read_dimacs(file);  // Pass file stream instead of filename

    // Measure memory before execution
    size_t mem_before = memory_used(true);

    // Run Dijkstra from a random source vertex
    int source = 0;  // Change this if you want a different source selection
    Dijkstra dijkstra(graph);
    dijkstra.computeShortestPaths(source, 2, true); // Assuming k=2 for K-ary Heap

    // Measure memory after execution
    size_t mem_after = memory_used(true);

    // Report results
    std::cout << "Memory used (before): " << mem_before / 1024 << " KB\n";
    std::cout << "Memory used (after): " << mem_after / 1024 << " KB\n";
    std::cout << "Memory increase: " << (mem_after - mem_before) / 1024 << " KB\n";
}

int main() {
    runDijkstraWithMemoryMeasurement("testes_escalonamento/NY.gr");
    runDijkstraWithMemoryMeasurement("testes_escalonamento/USA.gr");
    return 0;
}
