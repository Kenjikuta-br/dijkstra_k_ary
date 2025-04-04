#!/bin/bash

# Output directories
OUTPUT_DIR="input"
DIMACS_DIR="$OUTPUT_DIR/dimacs"
mkdir -p "$DIMACS_DIR"

# Path to the executable
gen_graph="./gen_graph"

# Check if the executable exists
if [[ ! -f $gen_graph ]]; then
    echo "Error: The executable '$gen_graph' was not found. Make sure it is in the current directory."
    exit 1
fi

# Generate graphs with m=2^10 and n=(4th_root(2))^i, i in [21, 35]
M=$((2**10))
echo "Generating graphs for m=$M with varying n"
for i in {21..35}; do
    N=$(echo "scale=10; e( ($i/4) * l(2) )" | bc -l | awk '{print int($1)}')  # Truncando N
    P=$(echo "$M / ($N * ($N - 1))" | bc -l)
    FILENAME="$DIMACS_DIR/graph_n${N}_m${M}.dimacs"
    echo "Executing: $gen_graph $N $P $FILENAME"
    $gen_graph $N $P $FILENAME
done

# Generate graphs with n=2^10 and m=sqrt(2)^i, i in [25, 39]
N=$((2**10))
echo "Generating graphs for n=$N with varying m"
for i in {25..39}; do
    M=$(echo "scale=10; e( ($i/2) * l(2) )" | bc -l | awk '{print int($1)}')  # Truncando M
    P=$(echo "$M / ($N * ($N - 1))" | bc -l)
    FILENAME="$DIMACS_DIR/graph_n${N}_m${M}.dimacs"
    echo "Executing: $gen_graph $N $P $FILENAME"
    $gen_graph $N $P $FILENAME
done

# Generate graphs with m=2^20 and n=sqrt(2)^i, i in [21, 35]
M=$((2**20))
echo "Generating graphs for m=$M with varying n"
for i in {21..35}; do
    N=$(echo "scale=10; e( ($i/2) * l(2) )" | bc -l | awk '{print int($1)}')  # Truncando N
    P=$(echo "$M / ($N * ($N - 1))" | bc -l)
    FILENAME="$DIMACS_DIR/graph_n${N}_m${M}.dimacs"
    echo "Executing: $gen_graph $N $P $FILENAME"
    $gen_graph $N $P $FILENAME
done

echo "All graphs have been generated in the directory: $DIMACS_DIR"