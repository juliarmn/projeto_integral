import pandas as pd

desvios = []

for i in range(1, 31):
    df = pd.read_csv(f'resultados{i}.csv')
    ultima_coluna = df.iloc[:, -1]
    desvios.append(ultima_coluna.values)

desvios_df = pd.DataFrame(desvios)

desvio_padrao_final = desvios_df.std(axis=0)

desvio_padrao_final.to_csv('desvios_resultados.csv', header=False, index=False)
