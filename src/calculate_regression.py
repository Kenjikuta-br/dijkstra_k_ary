import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression

# Suponha que esses sejam os dados coletados
dados = pd.DataFrame({
    "n": [2**11, 2**12, 2**13, 2**14, 2**15],
    "m": [2**20, 2**21, 2**22, 2**23, 2**24],
    "T": [0.5, 1.2, 3.1, 7.5, 15.3]  # Tempo de execução em segundos
})

# Aplicar log nos dados
dados["log_n"] = np.log(dados["n"])
dados["log_m"] = np.log(dados["m"])
dados["log_T"] = np.log(dados["T"])

# Criar modelo de regressão linear
X = dados[["log_n", "log_m"]]
y = dados["log_T"]

modelo = LinearRegression()
modelo.fit(X, y)

# Coeficientes encontrados
A = modelo.intercept_  # log(a)
b, c = modelo.coef_    # Coeficientes de log_n e log_m

# Exibir os resultados
print(f"log(a) = {A}, então a = {np.exp(A)}")
print(f"b = {b}")
print(f"c = {c}")
