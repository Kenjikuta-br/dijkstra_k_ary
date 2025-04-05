#include "dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <numeric>
#include <sstream>
#include <cstdint>

// Usa inteiro sem sinal de 128 bits para armazenar tamanhos de memória
__uint128_t memory_used(bool resident = false) {
    std::ifstream statm("/proc/self/statm");
    if (!statm) {
        std::cerr << "Erro ao abrir /proc/self/statm" << std::endl;
        return 0;
    }

    size_t pages;
    if (resident) {
        statm >> pages >> pages;  // Pega RSS
    } else {
        statm >> pages;           // Pega tamanho total
    }

    return static_cast<__uint128_t>(pages) * sysconf(_SC_PAGESIZE);
}

__int128_t extractExecutionTime(const std::string& output) {
    std::stringstream ss(output);
    std::string token;
    std::string last;

    while (std::getline(ss, token, ';')) {
        if (!token.empty())
            last = token;
    }

    try {
        return static_cast<__int128_t>(std::stoll(last));
    } catch (...) {
        return 0;
    }
}

void print128(__int128_t num) {
    if (num == 0) {
        std::cout << "0";
        return;
    }

    std::string result;
    bool negative = false;
    if (num < 0) {
        negative = true;
        num = -num;
    }

    while (num > 0) {
        result = char('0' + (num % 10)) + result;
        num /= 10;
    }

    if (negative) std::cout << "-";
    std::cout << result;
}

void runDijkstraWithMemoryMeasurement(const std::string& filename) {
    constexpr int numRuns = 5;
    constexpr int k = 17;
    constexpr int source = 0;

    std::vector<__int128_t> durations;
    std::vector<__uint128_t> memoryDeltas;

    int n = 0, m = 0;

    for (int i = 0; i < numRuns; ++i) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
            return;
        }

        Graph graph;
        graph.read_dimacs(file);
        n = graph.adj.size();
        m = graph.getNumEdges();

        __uint128_t mem_before = memory_used(true);

        // Redireciona a saída padrão para capturar os dados do Dijkstra
        std::streambuf* original_buf = std::cout.rdbuf();
        std::stringstream buffer;
        std::cout.rdbuf(buffer.rdbuf());

        Dijkstra dijkstra(graph);
        dijkstra.computeShortestPaths(source, k, true);

        std::cout.rdbuf(original_buf);  // Restaura o cout original

        __uint128_t mem_after = memory_used(true);
        __int128_t delta_bytes = static_cast<__int128_t>(mem_after) - static_cast<__int128_t>(mem_before);
        if (delta_bytes < 0) delta_bytes = 0;

        memoryDeltas.push_back(static_cast<__uint128_t>(delta_bytes) / 1024);  // em KB

        // Extrai o tempo de execução do buffer
        std::string captured = buffer.str();
        __int128_t exec_time = extractExecutionTime(captured);
        durations.push_back(exec_time);
    }

    __int128_t avg_time = std::accumulate(durations.begin(), durations.end(), __int128_t(0)) / numRuns;
    __uint128_t avg_mem = std::accumulate(memoryDeltas.begin(), memoryDeltas.end(), __uint128_t(0)) / numRuns;

    std::cout << filename << "," << n << "," << m << ",";
    print128(avg_time); std::cout << ",";
    print128(avg_mem); std::cout << "\n";
}

int main() {
    std::cout << "Graph,n,m,AvgTime(μs),AvgMemory(KB)\n";

    runDijkstraWithMemoryMeasurement("testes_escalonamento/NY.gr");
    runDijkstraWithMemoryMeasurement("testes_escalonamento/USA.gr");

    return 0;
}
