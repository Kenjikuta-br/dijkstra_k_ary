#!/bin/bash

# Configuration
PROGRAM="./bin/tester"
GRAPH_DIR="graphs/part2"
RESULTS_DIR="resultados/part2"
mkdir -p "$RESULTS_DIR"

K=17  # Fixed k value
NUM_RUNS=30  # Number of executions per graph

# Output CSV file
CSV_FILE="$RESULTS_DIR/normalized_results_k17.csv"

# CSV header with all fields
echo "k;vertices;edges;avg_insert;avg_extract;avg_decrease;avg_insert_r;avg_extract_r;avg_decrease_r;avg_time(microseconds);normalized_time" > "$CSV_FILE"

# Process both fixed_edge and fixed_vertice directories
for FIXED_TYPE in fixed_edge fixed_vertice; do
    echo "Processing $FIXED_TYPE graphs..."
    
    # Get all graph files sorted by vertices/edges
    while IFS= read -r -d $'\0' file; do
        # Extract vertex and edge counts from filename
        if [[ "$file" =~ graph_n([0-9]+)_m([0-9]+)\.gr ]]; then
            vertices="${BASH_REMATCH[1]}"
            edges="${BASH_REMATCH[2]}"
            
            echo "Running $NUM_RUNS tests for graph: $file with k=$K"
            
            # Execute the program and capture full output
            OUTPUT=$($PROGRAM "$K" "$NUM_RUNS" < "$file")
            
            # Parse all output fields
            IFS=';' read -r -a fields <<< "$OUTPUT"
            
            # Calculate normalized time: T/((n+m)*ln(n))
            if [[ "${fields[7]}" =~ ^[0-9.]+$ ]]; then
                avg_time="${fields[7]}"
                log_n=$(echo "l($vertices)" | bc -l)  # Natural logarithm
                n_plus_m=$(echo "$vertices + $edges" | bc)
                normalized_time=$(echo "scale=10; $avg_time / ($n_plus_m * $log_n)" | bc -l)
            else
                normalized_time="0"
            fi
            
            # Write complete data to CSV (fixed the field indices)
            echo "$K;$vertices;${fields[0]};${fields[1]};${fields[2]};${fields[3]};${fields[4]};${fields[5]};${fields[6]};${fields[7]};$normalized_time" >> "$CSV_FILE"
        fi
    done < <(find "$GRAPH_DIR/$FIXED_TYPE" -name "*.gr" -print0 | sort -zV)
done

echo "Execution completed. Results saved to $CSV_FILE"