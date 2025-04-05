import pandas as pd
import os
import numpy as np
from io import StringIO

# Caminhos e configurações
base_path = "resultados/part1"
output_csv = "aggregated_k17_metrics.csv"
densities = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]

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
        numeric_cols = ['p', 'k', 'vertices', 'edges', 'avg_time(microseconds)', 
                        'avg_insert', 'avg_extract', 'avg_decrease', 
                        'avg_insert_r', 'avg_extract_r', 'avg_decrease_r']
        for col in numeric_cols:
            df[col] = pd.to_numeric(df[col], errors='coerce')
        df_all.append(df)
    except Exception as e:
        print(f"Erro ao ler {file_path}: {e}")

# Concatenar todos os DataFrames
df_merged = pd.concat(df_all, ignore_index=True)

# Filtrar apenas as linhas onde k = 17
df_k17 = df_merged[df_merged['k'] == 17].copy()

# Calcular as métricas I, D e U
df_k17['I'] = df_k17['avg_insert'] / df_k17['vertices']
df_k17['D'] = df_k17['avg_extract'] / df_k17['vertices']
df_k17['U'] = df_k17['avg_decrease'] / df_k17['edges']


# Selecionar colunas relevantes para exportação
columns_to_export = [
    'p', 'k', 'vertices', 'edges', 'avg_insert', 'avg_extract', 'avg_decrease', 
    'avg_insert_r', 'avg_extract_r', 'avg_decrease_r', 
    'I', 'D', 'U', 'avg_time(microseconds)'
]
df_k17[columns_to_export].to_csv(output_csv, index=False)

print(f"Arquivo salvo: {output_csv}")
