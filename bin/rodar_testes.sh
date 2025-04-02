#!/bin/bash

PROGRAM="./bin/tester"  # Novo programa tester
GRAPH_DIR="graphs"
RESULTS_DIR="resultados"
mkdir -p "$RESULTS_DIR"

K_VALUES=$(echo {2..10} 64 128 256 512) 
NUM_RUNS=30  # Número de execuções por grafo/k

for DENSITY_FOLDER in "$GRAPH_DIR"/density_*; do
    DENSITY=$(basename "$DENSITY_FOLDER" | cut -d'_' -f2)
    CSV_FILE="$RESULTS_DIR/resultados_density_${DENSITY}.csv"
    
    # Cabeçalho do CSV atualizado
    echo "p;vertices;k;avg_insert;avg_extract;avg_decrease;avg_r;avg_time(microseconds);" > "$CSV_FILE"
    
    # Processamento dos arquivos (mantido igual)
    GRAPH_FILES=()
    while IFS= read -r -d $'\0' file; do
        vertices=$(basename "$file" | grep -oP '(?<=graph_)\d+(?=\.gr)')
        GRAPH_FILES+=("$vertices $file")
    done < <(find "$DENSITY_FOLDER" -name "graph_*.gr" -print0)
    
    IFS=$'\n' sorted=($(sort -n <<<"${GRAPH_FILES[*]}"))
    unset IFS
    
    GRAPH_FILES=()
    for item in "${sorted[@]}"; do
        GRAPH_FILES+=("${item#* }")
    done

    # Execução dos testes
    for GRAPH_FILE in "${GRAPH_FILES[@]}"; do
        VERTICES=$(basename "$GRAPH_FILE" | cut -d'_' -f2 | cut -d'.' -f1)
        
        for K in $K_VALUES; do
            echo "Executando $NUM_RUNS testes para p=$DENSITY, V=$VERTICES, k=$K"
            
            # Chama o novo programa tester que já faz as 30 execuções
            OUTPUT=$($PROGRAM "$K" "$NUM_RUNS" < "$GRAPH_FILE" )
            echo "$DENSITY;$VERTICES;$K;$OUTPUT" >> "$CSV_FILE"
        done
    done
done

echo "Execução concluída. Resultados salvos em $RESULTS_DIR"