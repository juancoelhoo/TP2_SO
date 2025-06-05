# -*- coding: utf-8 -*-

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_excel("resultados_simulador.xlsx")

df["arquivo"] = df["arquivo"].str.strip().str.lower()

arquivos_esperados = ["compilador", "compressor", "matriz", "simulador"]
algoritmos_esperados = sorted(df["algoritmo"].dropna().unique())

sns.set(style="whitegrid")
plt.rcParams["figure.figsize"] = (10, 6)

def plot_metric(metric, ylabel, title, filename, use_log = False):
    plt.figure()
    sns.barplot(
        data=df,
        x="algoritmo",
        y=metric,
        hue="arquivo",
        order=algoritmos_esperados,
        hue_order=arquivos_esperados,
        ci=None
    )
    if use_log:
        plt.yscale('log')
    plt.ylabel(ylabel)
    plt.xlabel("Algoritmo")
    plt.title(title)
    plt.legend(title="Arquivo de entrada", bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.savefig(filename)
    plt.close()

# 1. Page Faults
plot_metric("page_faults", "Page Faults (escala logaritmica)", "Page Faults por Algoritmo e Tabela", "page_faults.png", True)

# 2. Dirty Pages
plot_metric("dirty_pages", "Páginas Sujas (escala logaritmica)", "Páginas Sujas por Algoritmo e Tabela", "dirty_pages.png", True)

# 3. Substituições
plot_metric("substituicoes", "Substituições (escala logaritmica)", "Substituições por Algoritmo e Tabela", "substituicoes.png", True)

# 4. Memória Usada
plot_metric("memoria_usada_kb", "Memória Usada (KB)", "Uso de Memória por Estrutura de Tabela", "memoria_usada.png")

# 5. Tempo de Execução
plot_metric("tempo_execucao", "Tempo (s)", "Tempo de Execução por Algoritmo e Tabela", "tempo_execucao.png")

# 6. Taxa de Page Faults
plot_metric("fault_rate", "Taxa (%)", "Taxa de Page Faults por Algoritmo e Tabela", "fault_rate.png")

print("Gráficos salvos como arquivos .png!")
