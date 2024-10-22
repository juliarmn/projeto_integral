import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('planilha.csv', sep=',') 

data['média'] = data['média'].str.replace('.', '').str.replace(',', '.').astype(float)
data['resultado'] = data['resultado'].str.replace('.', '').astype(float)

grouped = data.groupby('threads')

max_média = data['média'].max() +1
min_média = 0 

for thread, group in grouped:
    plt.figure(figsize=(10, 6))
    plt.bar(group['intervalo_x'].astype(str) + ' | ' + group['intervalo_y'].astype(str), group['média'], color='pink')
    plt.title(f'Gráfico de Barras para {thread} Threads')
    plt.xlabel('Intervalo (X | Y)')
    plt.ylabel('média (segundos)')
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    plt.ylim(min_média, max_média)
    plt.savefig(f'grafico_threads_{thread}.png')
    plt.show()

