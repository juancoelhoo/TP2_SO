import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Carregar dados
df = pd.read_excel("resultados_simulador.xlsx")

# Configuração de estilo
sns.set(style="whitegrid")
plt.rcParams["figure.figsize"] = (10, 6)

# Função auxiliar
def plot_metric(metric, ylabel, title, filename, kind="bar"):
    plt.figure()
    sns.barplot(data=df, x="algoritmo", y=metric, hue="tabela", ci=None)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.tight_layout()
    plt.savefig(filename)
    plt.close()

# 1. Page Faults
plot_metric("page_faults", "Page Faults", "Page Faults por Algoritmo e Tabela", "page_faults.png")

# 2. Dirty Pages
plot_metric("dirty_pages", "Páginas Sujas", "Páginas Sujas por Algoritmo e Tabela", "dirty_pages.png")

# 3. Substituições
plot_metric("substituicoes", "Substituições", "Substituições por Algoritmo e Tabela", "substituicoes.png")

# 4. Memória Usada
plot_metric("memoria_usada_kb", "Memória Usada (KB)", "Uso de Memória por Estrutura de Tabela", "memoria_usada.png")

# 5. Tempo de Execução
plot_metric("tempo_execucao", "Tempo (s)", "Tempo de Execução por Algoritmo e Tabela", "tempo_execucao.png")

# 6. Taxa de Page Faults
plot_metric("fault_rate", "Taxa (%)", "Taxa de Page Faults por Algoritmo e Tabela", "fault_rate.png")

print("Gráficos salvos como arquivos .png!")
