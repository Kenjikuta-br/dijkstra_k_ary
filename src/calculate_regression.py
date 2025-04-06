import pandas as pd
import numpy as np
import statsmodels.api as sm
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler

# Carregar os dados
dados = pd.read_csv('resultados/part2/normalized_results_k17.csv', delimiter=';')

# Renomear colunas para facilitar o uso
dados = dados.rename(columns={
    'vertices': 'n',
    'edges': 'm',
    'avg_insert': 'I',
    'avg_extract': 'D',
    'avg_decrease': 'U',
    'avg_time(microseconds)': 'T'
})

# 1. Filtrar linhas com valores inválidos (zeros ou negativos)
dados = dados[(dados['n'] > 0) & 
             (dados['m'] > 0) & 
             (dados['T'] > 0) &
             (dados['I'] > 0) &
             (dados['D'] > 0)].copy()

# 2. Filtrar dados para análise específica
# Dados onde m é aproximadamente fixo (~1M arestas)
dados_m_fixo = dados[(dados['m'] > 1e6) & (dados['m'] < 1.1e6)].copy()

# Dados onde n é fixo (n=32768)
dados_n_fixo = dados[dados['n'] == 32768].copy()

# 3. Preparação para regressão linear
# Aplicar logaritmo nas variáveis (já filtramos zeros)
for df in [dados, dados_m_fixo, dados_n_fixo]:
    df['log_T'] = np.log(df['T'])
    df['log_n'] = np.log(df['n'])
    df['log_m'] = np.log(df['m'])

# 4. Regressão linear múltipla para todos os dados
X = dados[['log_n', 'log_m']]
X = sm.add_constant(X)  # Adiciona intercepto
y = dados['log_T']

model = sm.OLS(y, X)
results = model.fit()
print(results.summary())

# Extrair coeficientes
a = np.exp(results.params['const'])  # intercept
b = results.params['log_n']         # coeficiente para log_n
c = results.params['log_m']         # coeficiente para log_m

print(f"\nEquação estimada: T(n,m) = {a:.4f} * n^{b:.4f} * m^{c:.4f}")

# 5. Análise gráfica
plt.figure(figsize=(15, 5))

# Gráfico 1: Tempo normalizado vs número de vértices (m fixo)
plt.subplot(1, 3, 1)
plt.scatter(dados_m_fixo['n'], dados_m_fixo['normalized_time'])
plt.xscale('log')
plt.xlabel('Número de vértices (n)')
plt.ylabel('Tempo normalizado')
plt.title('Tempo normalizado vs n (m ≈ 1M)')
plt.grid(True)

# Gráfico 2: Tempo normalizado vs número de arestas (n fixo)
plt.subplot(1, 3, 2)
plt.scatter(dados_n_fixo['m'], dados_n_fixo['normalized_time'])
plt.xscale('log')
plt.xlabel('Número de arestas (m)')
plt.ylabel('Tempo normalizado')
plt.title('Tempo normalizado vs m (n=32768)')
plt.grid(True)

# Gráfico 3: Valores reais vs preditos
plt.subplot(1, 3, 3)
predicted = np.exp(results.predict(X))
plt.scatter(dados['T'], predicted)
plt.plot([min(dados['T']), max(dados['T'])], [min(dados['T']), max(dados['T'])], 'r--')
plt.xlabel('Tempo real (μs)')
plt.ylabel('Tempo predito (μs)')
plt.title('Valores reais vs preditos')
plt.grid(True)

plt.tight_layout()
plt.savefig('analise_dijkstra.png')  # Salvar figura antes de mostrar
plt.show()

# 6. Verificação dos limites das operações
print("\nVerificação dos limites:")
print(f"Max I/n: {max(dados['I']/dados['n'])} (deve ser <= 1)")
print(f"Max D/n: {max(dados['D']/dados['n'])} (deve ser <= 1)")
print(f"Max U/m: {max(dados['U']/dados['m'])} (deve ser <= 1)")

# 7. Análise das razões das operações
print("\nAnálise das razões médias:")
print(f"Insert ratio médio: {dados['avg_insert_r'].mean():.4f}")
print(f"Extract ratio médio: {dados['avg_extract_r'].mean():.4f}")
print(f"Decrease ratio médio: {dados['avg_decrease_r'].mean():.4f}")