#!/bin/bash

# Definir diretórios
SRC_DIR="src"
BIN_DIR="bin"
OUTPUT_DIR="graphs"

# Compilar o gerador de grafos
 g++ -o "$BIN_DIR/gen" "$SRC_DIR/gen.cpp" -std=c++17 -O2

# Verificar se a compilação foi bem-sucedida
if [ $? -ne 0 ]; then
    echo "Erro ao compilar gen.cpp"
    exit 1
fi

# Criar diretório de saída
mkdir -p "$OUTPUT_DIR"

# Gerar grafos para cada número de vértices
echo "Gerando grafos..."
for (( i=5; i<=13; i++ )); do
    n=$((2**i))
    output_file="$OUTPUT_DIR/graph_${n}.dimacs"
    echo "Gerando grafo com $n vértices..."
    "$BIN_DIR/gen" $n 0.09 > "$output_file"
done

echo "Todos os grafos foram gerados e salvos na pasta 'graphs'."