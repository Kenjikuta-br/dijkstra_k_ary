#include "dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <limits>

const int IDEAL_K = 4;

int main(int argc, char* argv[]) {
    // Verifica se o programa recebeu o parâmetro correto
    bool kenjiFlag = false;
    int source, destination, k;

    // Verifica argumentos e trata o caso do -kenji
    if (argc == 1) {
        std::cerr << "Usage: ./dijkstra <source> <destination> <k>(optional) [<] <graph_file>" << std::endl << "Colocar < para graph_file virar o stdin" << std::endl
                                                << "se colcoar -kenji no primeiro parâmetro, irá devolver outras informações, usadas para a coleta de dados" << std::endl;
        return 1;
    }

    // Se o argumento -kenji for encontrado, marca a flag
    if (std::string(argv[1]) == "-kenji") {
        //std::cout << "kenji" << std::endl;
        kenjiFlag = true;
        argc--; // Reduz o número de argumentos
        argv++; // Desloca os argumentos
    }

    // Assegura que os parâmetros restantes estão corretos
    if (argc != 3 && argc != 4) {
        std::cerr << "Usage: ./dijkstra <source> <destination> <k>(optional) [<] <graph_file>" << std::endl << "Colocar < para graph_file virar o stdin" << std::endl;
        return 1;
    }

    // Parse command-line arguments
    source = std::atoi(argv[1]) - 1; // Converte para índice 0-based
    destination = std::atoi(argv[2]) - 1; // Converte para índice 0-based
    k = (argc == 4) ? std::atoi(argv[3]) : IDEAL_K; // Usa IDEAL_K se k não for passado

    
    // Lê o grafo diretamente do stdin
    Graph graph;
    graph.read_dimacs(std::cin);

    // Cria uma instância de Dijkstra e calcula os caminhos mais curtos a partir da origem fornecida
    Dijkstra dijkstra(graph);
    dijkstra.computeShortestPaths(source, k, kenjiFlag); // Supondo que k=4 para a estrutura da heap

    // Obtém a distância mais curta
    int distance = dijkstra.getDistance(destination);

    // Imprime a distância mais curta, se não existir imprime 'inf'
    if (distance == std::numeric_limits<int>::max()) {
        std::cout << "inf" << std::endl;

    } else if (!kenjiFlag){ //
      std::cout << distance << std::endl;
    }

    return 0;
}