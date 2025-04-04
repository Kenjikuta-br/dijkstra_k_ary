import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Create directories if they don't exist
os.makedirs("charts/the_charts", exist_ok=True)

# Load the CSV data
df = pd.read_csv("resultados/part1/resultados_density_0.1.csv", sep=';')

# Clean up column names by stripping whitespace
df.columns = df.columns.str.strip()

# Filter to only include k values from 2 to 10
df = df[df['k'].between(2, 10)]

# Graph 1: Average time vs. k for each graph size
plt.figure(figsize=(12, 7))
sns.lineplot(data=df, x='k', y='avg_time(microseconds)', hue='vertices', 
             marker='o', palette='viridis', linewidth=2.5)
plt.title('Dijkstra Performance: Time vs Heap Arity (k=2-10)', fontsize=14)
plt.xlabel('Heap Arity (k)', fontsize=12)
plt.ylabel('Execution Time (μs)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(title='Vertices', title_fontsize=12, fontsize=10)
plt.savefig("charts/the_charts/avg_time_vs_k.png", dpi=300, bbox_inches='tight')
plt.close()

# Graph 2: Operation ratio analysis (simplified)
plt.figure(figsize=(10, 6))
sns.lineplot(data=df, x='k', y='avg_r', hue='vertices', 
             marker='o', palette='viridis', linewidth=2.5)
plt.title('Operation Efficiency Ratio vs Heap Arity (k=2-10)', fontsize=14)
plt.xlabel('Heap Arity (k)', fontsize=12)
plt.ylabel('Operation Ratio', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(title='Vertices', title_fontsize=12, fontsize=10)
plt.savefig("charts/the_charts/operation_ratio.png", dpi=300, bbox_inches='tight')
plt.close()

# Graph 3: Best k for each graph size (simplified)
plt.figure(figsize=(12, 6))
best_k = df.loc[df.groupby('vertices')['avg_time(microseconds)'].idxmin()]
sns.barplot(data=best_k, x='vertices', y='k', palette='viridis')
plt.title('Optimal Heap Arity (k) for Different Graph Sizes', fontsize=14)
plt.xlabel('Number of Vertices', fontsize=12)
plt.ylabel('Optimal k Value', fontsize=12)
plt.grid(True, axis='y', linestyle='--', alpha=0.7)
plt.savefig("charts/the_charts/optimal_k.png", dpi=300, bbox_inches='tight')
plt.close()

# Graph 4: Performance comparison (log scale)
plt.figure(figsize=(14, 7))
for k_val in sorted(df['k'].unique()):
    subset = df[df['k'] == k_val]
    sns.lineplot(data=subset, x='vertices', y='avg_time(microseconds)',
                 label=f'k={k_val}', marker='o', linewidth=2.5)

plt.title('Dijkstra Performance Across Graph Sizes (k=2-10)', fontsize=14)
plt.xlabel('Number of Vertices (log scale)', fontsize=12)
plt.ylabel('Execution Time (μs, log scale)', fontsize=12)
plt.xscale('log')
plt.yscale('log')
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.legend(title='Heap Arity (k)', title_fontsize=12, fontsize=10,
           bbox_to_anchor=(1.05, 1), loc='upper left')
plt.savefig("charts/the_charts/performance_comparison.png", dpi=300, bbox_inches='tight')
plt.close()

print("Refined graphs saved to 'charts/the_charts/'")