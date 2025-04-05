import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt

# Configure matplotlib for non-interactive environments
plt.switch_backend('Agg')  # Prevents the FigureCanvasAgg warning

# Load data and filter k=17
dados = pd.read_csv('resultados/part2/normalized_results_k17.csv', delimiter=';')
dados = dados[dados['k'] == 17]

# Remove rows with zero or negative values that would cause log problems
dados = dados[(dados['vertices'] > 0) & 
              (dados['edges'] > 0) & 
              (dados['avg_time(microseconds)'] > 0)]

# Create log columns safely
dados["log_n"] = np.log(dados["vertices"])
dados["log_m"] = np.log(dados["edges"])
dados["log_T"] = np.log(dados["avg_time(microseconds)"])

# Remove infinite values that may have been created
dados.replace([np.inf, -np.inf], np.nan, inplace=True)
dados.dropna(inplace=True)

def analyze_complexity(df, name):
    """Analyze time complexity for a dataset"""
    if len(df) < 2:  # Need at least 2 points for regression
        print(f"\nNot enough data points for {name}")
        return None
    
    try:
        X = df[["log_n", "log_m"]]
        y = df["log_T"]
        
        # Verify there are no infinite values
        if np.any(np.isinf(X.values)) or np.any(np.isinf(y.values)):
            print(f"\nInfinite values found in {name}, skipping")
            return None
            
        modelo = LinearRegression()
        modelo.fit(X, y)
        
        A = modelo.intercept_
        b, c = modelo.coef_
        
        print(f"\nAnálise para {name}:")
        print(f"log(a) = {A:.4f}, então a ≈ {np.exp(A):.4f}")
        print(f"Coeficiente para n (b) = {b:.4f}")
        print(f"Coeficiente para m (c) = {c:.4f}")
        print(f"Equação estimada: log(T) = {A:.4f} + {b:.4f}*log(n) + {c:.4f}*log(m)")
        print(f"Ou: T ≈ {np.exp(A):.4f} * n^{b:.4f} * m^{c:.4f}")
        
        # Plot results
        plt.figure(figsize=(10, 6))
        plt.scatter(df["vertices"], df["avg_time(microseconds)"], label='Dados observados')
        
        # Add prediction line
        df = df.copy()  # Avoid SettingWithCopyWarning
        df.loc[:, 'predicted'] = np.exp(modelo.predict(X))
        plt.plot(df["vertices"], df['predicted'], 'r-', label='Modelo ajustado')
        
        plt.xscale('log')
        plt.yscale('log')
        plt.xlabel('Número de vértices (n)')
        plt.ylabel('Tempo de execução (μs)')
        plt.title(f'Complexidade do Algoritmo ({name})')
        plt.legend()
        plt.grid(True)
        plt.savefig(f'complexity_analysis_{name}.png')
        plt.close()
        
        return modelo
    
    except Exception as e:
        print(f"\nError analyzing {name}: {str(e)}")
        return None

# Separate data
fixed_vertices = dados[dados['edges'] > 1e6]  # Graphs with fixed vertices
fixed_edges = dados[dados['edges'].between(0.9e6, 1.1e6)]  # Graphs with ~1M edges

# Run analyses
print("="*60)
modelo_vertices = analyze_complexity(fixed_vertices, "Vértices Fixos")
print("="*60)
modelo_arestas = analyze_complexity(fixed_edges, "Arestas Fixas")
print("="*60)
modelo_geral = analyze_complexity(dados, "Todos os dados")
print("="*60)