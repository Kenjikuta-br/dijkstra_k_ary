#!/bin/bash

PROGRAM="./bin/dijkstra"  # Substitua pelo caminho do seu executável
GRAPH_DIR="graphs"  # Diretório onde os grafos estão armazenados
RESULTS_DIR="resultados"  # Diretório para salvar os resultados
mkdir -p "$RESULTS_DIR"

K_VALUES=$(seq 2 1 10)  # Valores de k

# Itera sobre as pastas de densidade
for DENSITY_FOLDER in "$GRAPH_DIR"/density_*; do
    DENSITY=$(basename "$DENSITY_FOLDER" | cut -d'_' -f2)  # Extrai o valor de p
    CSV_FILE="$RESULTS_DIR/resultados_density_${DENSITY}.csv"
    echo "p;vertices;k;insert_count;extractMin_count;decreaseKey_count;avg_r;execution_time" > "$CSV_FILE"  # Cabeçalho do CSV
    
    # Itera sobre os arquivos de grafo na pasta
    for GRAPH_FILE in "$DENSITY_FOLDER"/graph_*.gr; do
        VERTICES=$(basename "$GRAPH_FILE" | cut -d'_' -f2 | cut -d'.' -f1)  # Extrai o número de vértices
        
        for K in $K_VALUES; do
            echo "Executando Dijkstra para p=$DENSITY, V=$VERTICES, k=$K"
            OUTPUT=$($PROGRAM "-kenji" "1" "5" "$K" < "$GRAPH_FILE")
            echo "$DENSITY;$VERTICES;$K;$OUTPUT" >> "$CSV_FILE"
        done
    done

done

echo "Execução concluída. Resultados salvos em $RESULTS_DIR"

