#!/bin/bash

#chmod +x bin/rodar_testes.sh

# Diretório onde seu código está localizado (ajuste conforme necessário)
PROGRAM="./bin/dijkstra"  # Substitua pelo nome do seu programa executável

# Lista de arquivos de entrada (grafos em arquivos .txt)
INPUT_FILES=("testes/grafo_teste1.txt" "testes/grafo_teste2.txt")  # Adicione os arquivos de grafos que você deseja testar

# Valores de k para testar 
K_VALUES=$(seq 2 1 10)

# Origem e Destino para o algoritmo Dijkstra (ajuste conforme necessário)
SOURCE=1  # Origem (1-based)
DESTINATION=5  # Destino (1-based)

# Criar um diretório para armazenar os resultados
RESULTS_DIR="resultados"
mkdir -p "$RESULTS_DIR"

# Loop sobre os arquivos de entrada e os valores de k
for FILE in "${INPUT_FILES[@]}"; do
    for K in $K_VALUES; do
        # Executar o programa com o grafo atual e o valor de k
        echo "Executando Dijkstra com k=$K no arquivo $FILE"
        
        # Nome do arquivo de saída para os resultados
        OUTPUT_FILE="$RESULTS_DIR/resultado_$(basename "$FILE" .txt)_k${K}.txt"
        
        # Chamar o programa, passando os parâmetros (origem, destino, k)
        # O arquivo de entrada será redirecionado para o programa Dijkstra
        $PROGRAM "$SOURCE" "$DESTINATION" "$K" < "$FILE" > "$OUTPUT_FILE"
        
        echo "Resultado salvo em $OUTPUT_FILE"
    done
done

echo "Execução concluída."
