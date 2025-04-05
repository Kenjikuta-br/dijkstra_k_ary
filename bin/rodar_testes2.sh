#!/bin/bash

# Configuration
PROGRAM="./bin/tester"
GRAPH_DIR="graphs/part2"
RESULTS_DIR="resultados/part2"
mkdir -p "$RESULTS_DIR"

K=17  # Fixed k value
NUM_RUNS=20  # Number of executions per graph

# Output CSV file
CSV_FILE="$RESULTS_DIR/normalized_results_k17.csv"

# CSV header with normalized time column
echo "k;vertices;edges;avg_insert;avg_extract;avg_decrease;avg_insert_r;avg_extract_r;avg_decrease_r;avg_time(microseconds);normalized_time" > "$CSV_FILE"

# Process both fixed_edge and fixed_vertices directories
for FIXED_TYPE in fixed_edge fixed_vertices; do
    echo "Processing $FIXED_TYPE graphs..."
    
    # Get all graph files sorted by vertices/edges
    GRAPH_FILES=()
    while IFS= read -r -d $'\0' file; do
        # Extract vertex count from filename
        if [ "$FIXED_TYPE" = "fixed_edge" ]; then
            vertices=$(basename "$file" | grep -oP 'n\K\d+(?=_m)')
            edges=$(basename "$file" | grep -oP 'm\K\d+(?=\.gr)')
        else
            vertices=$(basename "$file" | grep -oP 'n\K\d+(?=_m)')
            edges=$(basename "$file" | grep -oP 'm\K\d+(?=\.gr)')
        fi
        GRAPH_FILES+=("$vertices $edges $file")
    done < <(find "$GRAPH_DIR/$FIXED_TYPE" -name "*.gr" -print0 | sort -zV)
    
    # Process each graph file
    for graph_info in "${GRAPH_FILES[@]}"; do
        read vertices edges file <<< "$graph_info"
        
        echo "Running $NUM_RUNS tests for graph: $file with k=$K"
        
        # Execute the program and capture output
        OUTPUT=$($PROGRAM "$K" "$NUM_RUNS" < "$file")
        
        # Calculate normalized time: T / ((n+m)*log(n))
        if [[ $OUTPUT =~ ([0-9.]+); ]]; then
            avg_time=${BASH_REMATCH[1]}
            log_n=$(echo "l($vertices)/l(2)" | bc -l)  # log2(n)
            n_plus_m=$(echo "$vertices + $edges" | bc)
            normalized_time=$(echo "$avg_time / ($n_plus_m * $log_n)" | bc -l)
            
            # Append normalized time to the output
            OUTPUT="${OUTPUT}${normalized_time}"
        else
            OUTPUT="${OUTPUT}0"  # Default if calculation fails
        fi
        
        # Write to CSV (using density=0.1 as placeholder, adjust if needed)
        echo "$K;$vertices;$edges;$OUTPUT" >> "$CSV_FILE"
    done
done

echo "Execution completed. Results saved to $CSV_FILE"