#!/bin/bash

#chmod +x bin/criar_grafos.sh

# Definir diretórios
SRC_DIR="src"
BIN_DIR="bin"
OUTPUT_DIR="graphs/density_0,8"

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
for (( i=5; i<=12; i++ )); do
    n=$((2**i))
    output_file="$OUTPUT_DIR/graph_${n}.gr"
    echo "Gerando grafo com $n vértices..."
    "$BIN_DIR/gen" $n 0.8 > "$output_file"
done

echo "Todos os grafos foram gerados e salvos na pasta 'graphs'."