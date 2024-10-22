import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('planilha.csv', sep=',') 

df['média'] = df['média'].str.replace(',', '.').astype(float)
df['desvio'] = df['desvio'].str.replace(',', '.').astype(float)

plt.figure(figsize=(10, 5))
plt.bar(df['threads'], df['média'], color='orange')
plt.title('Pior caso: Tempo (segundos) vs Número de threads')
plt.ylabel('Threads')
plt.xlabel('Tempo (segundos)')
plt.xticks(df['threads']) 
plt.grid(axis='y')
plt.show()
