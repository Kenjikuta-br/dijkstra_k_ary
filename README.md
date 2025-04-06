# Dijkstra k-ary

Esta é uma implementação do algoritmo de Dijkstra utilizando um heap k-ário, desenvolvida em C++ e acompanhada de scripts em Python para análise dos resultados. O objetivo principal deste projeto é validar experimentalmente que a complexidade de tempo do algoritmo é de fato O((n + m) log n), onde n é o número de vértices e m o número de arestas do grafo.

## Visão Geral

- **Implementação:**  
  O algoritmo de Dijkstra foi implementado em C++ utilizando uma fila de prioridades baseada em um heap k-ário. A escolha do valor de *k* foi determinada por meio de testes experimentais, onde se avaliou o desempenho variando *k* de 2 a 64. Para otimizar a execução, foram utilizadas estruturas auxiliares (vetores para hashing por índice) que possibilitam buscas em tempo constante O(1).

- **Geração de Grafos:**  
  Os grafos foram gerados a partir de um código adaptado de um gerador no formato DIMACS. Os parâmetros de entrada são o número de vértices (utilizando quantidades de vértices definidas por 2^i, com i ∈ [8,15]) e a probabilidade *p* de criação de uma aresta entre cada par de vértices. Dois conjuntos de dados foram gerados (um com número fixo de vértices e outro com número fixo de arestas) para possibilitar comparações e insights sobre a escolha ideal do parâmetro *k*.

- **Análise Experimental:**  
  Foram realizados 30 testes para cada instância de grafo, utilizando o mesmo vértice de origem. O tempo de execução (medido em microssegundos) e o uso de memória (calculado via RSS, utilizando o arquivo `/proc/self/statm`) foram os principais indicadores analisados. A análise também incluiu a contagem de operações elementares no heap (sift-up, sift-down) e uma regressão linear log-log para modelar o tempo de execução em função de n e m.

## Ambiente de Testes

- **Hardware Utilizado:**  
  - Processador: AMD Ryzen 5 5600X (3.7GHz base, 4.6GHz Turbo, 6 núcleos, 12 threads, cache 35MB, plataforma AM4)  
  - Placa-mãe: Asus TUF Gaming X570-Plus  
  - Memória RAM: 32GB DDR4 (XPG Spectrix D50 RGB, 3000 MHz e 3200 MHz)  
  - Armazenamento: SSD WD Black SN750 500GB NVMe (3430MB/s leitura, 2600MB/s gravação)  
  - Placa de vídeo: Zotac Gaming NVIDIA GeForce RTX 2060 (6GB GDDR6)

- **Software Utilizado:**  
  - **Sistema Operacional:** Ubuntu 24.04.2 LTS (codinome *noble*)  
  - **Compilador:** GCC 13.3.0  
  - **Linguagem de Programação:** C++ (para a implementação do algoritmo)  
  - **Scripts de Análise:** Python 3.12.3 (em ambiente virtual `venv`) com as bibliotecas: `pandas`, `numpy`, `statsmodels`, `matplotlib`, `seaborn`, `scikit-learn` (entre outras utilitárias)

## Estrutura do Repositório

A organização das pastas é a seguinte:

dijkstra_k_ary/
├── .vscode/            
│   ├── c_cpp_properties.json  → Configurações do IntelliSense, include paths e compilação para C/C++ no VS Code.
│   ├── settings.json          → Configurações específicas do workspace.
│   └── tasks.json             → Definições de tarefas (build, execução, etc.) para automação no VS Code.
├── bin/                
│   ├── criar_grafos.sh        → Script para gerar grafos automaticamente.
│   ├── criar_grafos2.sh       → Variante do script para criação de grafos.
│   ├── dijkstra               → Executável da implementação do algoritmo de Dijkstra.
│   ├── dijkstra_mem           → Versão do executável que pode incluir monitoramento de memória.
│   ├── gen                    → Executável usado para geração de dados ou grafos.
│   ├── rodar_testes.sh        → Script para executar os testes do projeto.
│   ├── rodar_testes2.sh       → Alternativa para execução dos testes.
│   └── tester                 → Ferramenta executável para testes específicos.
├── charts/             
│   ├── chart_generator        → Código responsável por gerar gráficos.
│   ├── the_charts             → Gráficos gerados.
│   └── the_charts2            → Outra coleção de gráficos.
├── include/            
│   ├── dijkstra.hpp           → Declarações do algoritmo.
│   ├── graph.hpp              → Estruturas de grafos.
│   ├── heap_node.hpp          → Nó usado nos heaps.
│   └── k_ary_heap.hpp         → Implementação do heap k-ário.
├── obj/                
│   ├── dijkstra.o             → Objeto compilado do código Dijkstra.
│   ├── graph.o                → Objeto dos grafos.
│   ├── k_ary_heap.o           → Objeto do heap k-ário.
│   └── main.o                 → Objeto do main.cpp.
├── resultados/         
│   ├── part1                  → Resultados dos testes.
│   └── part2                  → Mais resultados.
├── src/                
│   ├── aggregate_csv_k17.py   → Agrega métricas.
│   ├── calculate_regression.py→ Cálculos de regressão.
│   ├── dijkstra.cpp           → Implementação.
│   ├── dijkstra_mem.cpp       → Variante com memória.
│   ├── gen.cpp                → Geração de grafos.
│   ├── graph.cpp              → Estruturas de grafos.
│   ├── k_ary_heap.cpp         → Heap k-ário.
│   ├── main.cpp               → Ponto de entrada.
│   └── tester.cpp             → Testes.
├── testes_escalonamento/ 
│   └── NY.gr                  → Grafo de testes.
├── .gitignore                 → Arquivos ignorados.
├── Makefile                   → Build script.
└── aggregated_k17_metrics.csv → Métricas agregadas.


# Ambiente Virtual com Python

Este projeto utiliza um ambiente virtual para gerenciar as dependências.

## 🔧 Configuração do Ambiente Virtual

Para configurar o ambiente virtual e instalar as dependências, siga os passos abaixo:

```bash
# 1. Crie o ambiente virtual
python3 -m venv venv

# 2. Ative o ambiente virtual

# No Linux/macOS:
source venv/bin/activate

# 3. Instale as dependências listadas no requirements.txt
pip install -r requirements.txt

# 4. (Opcional) Verifique os pacotes instalados
pip list

# 5. Quando terminar, desative o ambiente virtual
deactivate


