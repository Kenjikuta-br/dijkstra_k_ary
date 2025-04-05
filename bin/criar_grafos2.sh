#!/bin/bash


#aqui estou dando crédito para meu amigo henrique que me ajudou com grande parte desse código em bash para gerar minha segunda isntaância de datasets com N e M fixos
#obrigado henrique!

# Output directories
DIMACS_DIR="graphs/part2"
mkdir -p "$DIMACS_DIR"

# Path to the executable
gen_graph="bin/gen"

# Check if the executable exists
if [[ ! -f $gen_graph ]]; then
    echo "Error: The executable '$gen_graph' was not found. Make sure it is in the current directory."
    exit 1
fi


# Generate graphs with n=2^10 and m=sqrt(2)^i, i in [25, 39]
N=$((2**15))
echo "Generating graphs for n=$N with varying m"
for i in {30..44}; do
    M=$(echo "scale=10; e( ($i/2) * l(2) )" | bc -l | awk '{print int($1)}')  # Truncando M
    P=$(echo "$M / ($N * ($N - 1))" | bc -l)
    FILENAME="$DIMACS_DIR/fixed_vertice/graph_n${N}_m${M}.gr"
    echo "Executing: $gen_graph $N $P > "$FILENAME""
    $gen_graph $N $P > "$FILENAME"
done

# Generate graphs with m=2^20 and n=sqrt(2)^i, i in [21, 35]
M=$((2**20))
echo "Generating graphs for m=$M with varying n"
for i in {21..35}; do
    N=$(echo "scale=10; e( ($i/2) * l(2) )" | bc -l | awk '{print int($1)}') 
    P=$(echo "$M / ($N * ($N - 1))" | bc -l)
    FILENAME="$DIMACS_DIR/fixed_edge/graph_n${N}_m${M}.gr"
    echo "Executing: $gen_graph $N $P > "$FILENAME""
    $gen_graph $N $P > "$FILENAME"
done


echo "All graphs have been generated in the directory: $DIMACS_DIR"