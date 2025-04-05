import pandas as pd
import matplotlib.pyplot as plt
import os

# Caminho para salvar os gráficos
output_dir = 'charts/the_charts2'
os.makedirs(output_dir, exist_ok=True)

# Lê o CSV com delimitador ';'
df = pd.read_csv('resultados/part2/normalized_results_k17.csv', delimiter=';')

# Filtra apenas os dados com número de vértices fixo
v_fixo = 32768
df_vfixo = df[df['vertices'] == v_fixo]

# GRÁFICO 1: Operações por nó normalizadas vs número de arestas
plt.figure(figsize=(10, 6))
plt.plot(df_vfixo['edges'], df_vfixo['avg_insert_r'], label='avg_insert_r', marker='o')
plt.plot(df_vfixo['edges'], df_vfixo['avg_extract_r'], label='avg_extract_r', marker='s')
plt.plot(df_vfixo['edges'], df_vfixo['avg_decrease_r'], label='avg_decrease_r', marker='^')

plt.xscale('log')
plt.xlabel('Número de Arestas (log)')
plt.ylabel('Operações por nó (normalizadas)')
plt.title(f'Operações por nó normalizadas vs Número de Arestas (v = {v_fixo})')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(output_dir, 'grafico_operacoes_por_no.png'))
plt.close()

# GRÁFICO 2: Tempo normalizado vs número de arestas
plt.figure(figsize=(10, 6))
plt.plot(df_vfixo['edges'], df_vfixo['normalized_time'], label='tempo normalizado', marker='d', color='purple')

plt.xscale('log')
plt.xlabel('Número de Arestas (log)')
plt.ylabel('Tempo Normalizado')
plt.title(f'Tempo Normalizado vs Número de Arestas (v = {v_fixo})')
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(output_dir, 'grafico_tempo_normalizado.png'))
plt.close()


# Define target edge count and tolerance (5%)
target_edges = 1048576
tolerance = 0.05  # 5% tolerance

# Calculate lower and upper bounds
lower_bound = target_edges * (1 - tolerance)
upper_bound = target_edges * (1 + tolerance)

# Filter data for graphs with edges within the tolerance range
df_mfixo = df[(df['edges'] >= lower_bound) & (df['edges'] <= upper_bound)]

# Sort by vertices to ensure proper plotting order
df_mfixo = df_mfixo.sort_values('vertices')

# GRÁFICO 3: Operações por nó normalizadas vs número de vértices (arestas ~fixas)
plt.figure(figsize=(10, 6))
plt.plot(df_mfixo['vertices'], df_mfixo['avg_insert_r'], label='avg_insert_r', marker='o')
plt.plot(df_mfixo['vertices'], df_mfixo['avg_extract_r'], label='avg_extract_r', marker='s')
plt.plot(df_mfixo['vertices'], df_mfixo['avg_decrease_r'], label='avg_decrease_r', marker='^')

plt.xscale('log')
plt.xlabel('Número de Vértices (log)')
plt.ylabel('Operações por nó (normalizadas)')
plt.title(f'Operações por nó normalizadas vs Número de Vértices\n(arestas ≈ {target_edges:,}, ±{tolerance*100:.0f}%)'.replace(',', '.'))
plt.legend()
plt.grid(True, which='both', linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig(os.path.join(output_dir, 'grafico_operacoes_por_no_arestas_proximas.png'))
plt.close()

# GRÁFICO 4: Tempo normalizado vs número de vértices (arestas ~fixas)
plt.figure(figsize=(10, 6))
plt.plot(df_mfixo['vertices'], df_mfixo['normalized_time'], 
         label='tempo normalizado', marker='d', color='purple')

plt.xscale('log')
plt.xlabel('Número de Vértices (log)')
plt.ylabel('Tempo Normalizado')
plt.title(f'Tempo Normalizado vs Número de Vértices\n(arestas ≈ {target_edges:,}, ±{tolerance*100:.0f}%)'.replace(',', '.'))
plt.grid(True, which='both', linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig(os.path.join(output_dir, 'grafico_tempo_normalizado_arestas_proximas.png'))
plt.close()