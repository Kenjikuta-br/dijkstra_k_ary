#!/bin/bash

#chmod +x bin/criar_grafos.sh

# Definir diretórios
SRC_DIR="src"
BIN_DIR="bin"
BASE_OUTPUT_DIR="graphs/part1"

# Densidades a serem usadas
densities=("0.1" "0.2" "0.3" "0.4" "0.5" "0.6")

# Compilar o gerador de grafos
g++ -o "$BIN_DIR/gen" "$SRC_DIR/gen.cpp" -std=c++17 -O2

# Verificar se a compilação foi bem-sucedida
if [ $? -ne 0 ]; then
    echo "Erro ao compilar gen.cpp"
    exit 1
fi

# Gerar grafos para cada densidade
for density in "${densities[@]}"; do
    # Criar diretório de saída para a densidade
    OUTPUT_DIR="$BASE_OUTPUT_DIR/density_$density"
    mkdir -p "$OUTPUT_DIR"

    echo "Gerando grafos com densidade $density..."

    # Gerar grafos para cada número de vértices
    for ((i=6; i<=13; i++)); do
        n=$((2**i))
        output_file="$OUTPUT_DIR/graph_${n}.gr"
        echo "Gerando grafo com $n vértices e densidade $density..."
        "$BIN_DIR/gen" $n $density > "$output_file"
    done
done

echo "Todos os grafos foram gerados e salvos nas pastas correspondentes às densidades."
