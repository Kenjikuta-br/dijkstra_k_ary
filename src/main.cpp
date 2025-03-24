#include "dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>

int main(int argc, char* argv[]) {
    // Ensure correct number of arguments
    if (argc != 4) {
        std::cerr << "Usage: ./dijkstra <source> <destination>  <k>   <   <graph_file>" << std::endl;
        return 1;
    }

    // Parse command-line arguments
    int source = std::atoi(argv[1]) - 1; // Convert to 0-based index
    int destination = std::atoi(argv[2]) - 1; // Convert to 0-based index
    int k = std::atoi(argv[3]);

    // Read graph from standard input
    Graph graph;
    graph.read_dimacs(std::cin);

    // Create a Dijkstra instance and compute the shortest paths from the given source
    Dijkstra dijkstra(graph);
    dijkstra.computeShortestPaths(source, k); // Assuming k=4 for the heap structure

    // Get the shortest distance
    int distance = dijkstra.getDistance(destination);

    // Print the shortest distance from source to destination, if it doesn't exist print inf
    if (distance == std::numeric_limits<int>::max()) {
        std::cout << "inf" << std::endl;
    } else {
        std::cout << "Distância de " << (source + 1) << " até " << (destination + 1) << ": " << distance << std::endl;
    }

    return 0;
}
