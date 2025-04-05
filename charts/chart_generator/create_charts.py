import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import numpy as np
from io import StringIO

# Caminhos e configurações
base_path = "resultados/part1"
output_dir = "charts/the_charts"
densities = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]
x_ticks = [2, 8,16,22,24,32,40,48,56,64]

# Criar diretório de saída se necessário
os.makedirs(output_dir, exist_ok=True)

# Lista para armazenar todos os DataFrames
df_all = []

# Ler todos os arquivos
for p in densities:
    file_path = os.path.join(base_path, f"resultados_density_{p}.csv")
    try:
        with open(file_path, 'r') as f:
            lines = f.readlines()
        cleaned_lines = [line.strip().rstrip(';') for line in lines]
        df = pd.read_csv(StringIO('\n'.join(cleaned_lines)), delimiter=';')
        df.columns = [
            'p', 'k', 'vertices', 'edges', 'avg_insert', 'avg_extract', 
            'avg_decrease', 'avg_insert_r', 'avg_extract_r', 'avg_decrease_r', 
            'avg_time(microseconds)'
        ]
        numeric_cols = ['p', 'k', 'vertices', 'edges', 'avg_time(microseconds)']
        for col in numeric_cols:
            df[col] = pd.to_numeric(df[col], errors='coerce')
        df_all.append(df)
    except Exception as e:
        print(f"Erro ao ler {file_path}: {e}")

# Concatenar todos os DataFrames
df_merged = pd.concat(df_all, ignore_index=True)

# Filtrar apenas para vértices = 64 e k nos valores desejados
df_64 = df_merged[(df_merged['vertices'] == 64)]

# Criar um novo DataFrame sem restringir o número de vértices
df_all_vertices = df_merged[df_merged['k'].isin(x_ticks)]

# Estilo dos gráficos
plt.style.use('default')
sns.set_theme(style="whitegrid", palette="husl")
plt.rcParams['figure.facecolor'] = 'white'
plt.rcParams['axes.grid'] = True
plt.rcParams['grid.linestyle'] = '--'
plt.rcParams['grid.alpha'] = 0.7

# GRÁFICOS COMBINADOS

# 1. Linha: tempo médio por k para cada p
# Paleta de cores altamente contrastante
cores_vibrantes = [
    "#FF0000", "#0000FF", "#00FF00", "#FFA500", 
    "#800080", "#00FFFF", "#FF00FF", "#808000",
    "#008000", "#000080", "#8B0000", "#FFD700"
]

plt.figure(figsize=(10, 6))
sns.set_palette(cores_vibrantes)  # Aplicando a paleta manualmente

sns.lineplot(data=df_64, x='k', y='avg_time(microseconds)', hue='p',
             marker='o', linewidth=2.5)

plt.title('Tempo médio vs k (por densidade p)', pad=20)
plt.xlabel('Valor de k', labelpad=10)
plt.ylabel('Tempo médio (μs)', labelpad=10)
plt.xticks(x_ticks)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(title='Densidade (p)')
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "tempo_vs_k_por_densidade_linha.png"),
            bbox_inches='tight', dpi=300)
plt.close()


# 2. Dispersão com regressão separada por p
plt.figure(figsize=(10, 6))
sns.lmplot(data=df_64, x='k', y='avg_time(microseconds)', hue='p',
           markers='o', aspect=1.5, height=6, scatter_kws={'s': 80, 'alpha': 0.8})
plt.suptitle('Tendência de tempo por k com regressão (por densidade)', y=1.02)
plt.xticks(x_ticks)
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig(os.path.join(output_dir, "tempo_vs_k_por_densidade_regressao.png"),
            bbox_inches='tight', dpi=300)
plt.close()

# Agrupar por k e tirar a média dos tempos
df_k_avg = df_merged.groupby('k', as_index=False)['avg_time(microseconds)'].mean()

# 3. Linha agregada: Tempo médio vs k (média geral de p e vértices)
plt.figure(figsize=(10, 6))
sns.lineplot(data=df_k_avg, x='k', y='avg_time(microseconds)', marker='o', linewidth=2.5, color='steelblue')

plt.title('Tempo médio vs k (média geral de p e vértices)', pad=20)
plt.xlabel('Valor de k', labelpad=10)
plt.ylabel('Tempo médio (μs)', labelpad=10)

# Mostra apenas alguns ticks no eixo x, mas mantém todos os pontos no gráfico
plt.xticks(x_ticks)

plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "tempo_vs_k_geral.png"),
            bbox_inches='tight', dpi=300)
plt.close()

# FILTRAGEM POR p = 0.1
df_p01 = df_merged[df_merged['p'] == 0.1]

# 4. Tempo médio por k (considerando todos os grafos com p = 0.1)
df_k_avg_p01 = df_p01.groupby('k', as_index=False)['avg_time(microseconds)'].mean()

plt.figure(figsize=(10, 6))
sns.lineplot(data=df_k_avg_p01, x='k', y='avg_time(microseconds)', marker='o', linewidth=2.5, color='darkgreen')
plt.title('Tempo médio vs k (p = 0.1, média de todos os grafos)', pad=20)
plt.xlabel('Valor de k', labelpad=10)
plt.ylabel('Tempo médio (μs)', labelpad=10)
plt.xticks(x_ticks)
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "tempo_vs_k_p01_media.png"),
            bbox_inches='tight', dpi=300)
plt.close()

# Valores distintos de número de vértices (esperados: 64 até 8192, potências de 2)
vertex_sizes = sorted(df_p01['vertices'].unique())

# 5. Um gráfico por número de vértices (p = 0.1)
for v in vertex_sizes:
    df_v = df_p01[df_p01['vertices'] == v]
    
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df_v, x='k', y='avg_time(microseconds)', marker='o', linewidth=2.5, color='tab:blue')
    plt.title(f'Tempo vs k (p = 0.1, vértices = {v})', pad=20)
    plt.xlabel('Valor de k', labelpad=10)
    plt.ylabel('Tempo médio (μs)', labelpad=10)
    plt.xticks(x_ticks)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    filename = f"tempo_vs_k_p01_vertices_{v}.png"
    plt.savefig(os.path.join(output_dir, filename), bbox_inches='tight', dpi=300)
    plt.close()

print("\nGráficos por densidade salvos com sucesso!")
