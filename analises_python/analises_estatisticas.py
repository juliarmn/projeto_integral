import pandas as pd

medias = []

for i in range(1, 31):
    df = pd.read_csv(f'resultados{i}.csv')
    ultima_coluna = df.iloc[:, -1]
    medias.append(ultima_coluna.values)

medias_df = pd.DataFrame(medias)

media_final = medias_df.mean(axis=0)
media_final.to_csv('medias_resultados.csv', header=False, index=False)
