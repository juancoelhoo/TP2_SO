import os
import re
import pandas as pd

# Caminho da pasta com os arquivos .txt
PASTA_RESULTADOS = "../resultados"
ARQUIVO_SAIDA = "../graphs/resultados_simulador.xlsx"

# Padrões para extrair os dados dos .txt
regex_mappings = {
    "algoritmo_tabela": r"Teste: (\S+) com",
    "arquivo": r"com (\S+)\.txt",
    "total_acessos": r"Total de acessos: (\d+)",
    "page_faults": r"Páginas lidas \(page faults\): (\d+)",
    "dirty_pages": r"Páginas escritas \(dirty pages\): (\d+)",
    "paginas_unicas": r"Número de páginas únicas acessadas: (\d+)",
    "substituicoes": r"Número de substituições de página: (\d+)",
    "fault_rate": r"Taxa de page faults: ([\d.]+)%",
    "dirty_rate": r"Taxa de páginas sujas: ([\d.]+)%",
    "memoria_usada_kb": r"Memória usada pelas tabelas: \d+ bytes \(([\d.]+) KB\)",
    "tempo_execucao": r"Tempo de execução: ([\d.]+) segundos",
    "tamanho_memoria": r"Tamanho da memória: (\d+) KB",
    "tamanho_pagina": r"Tamanho das páginas: (\d+) KB"
}

def extrair_dados_arquivo(caminho):
    with open(caminho, "r", encoding="utf-8") as f:
        conteudo = f.read()

    dados = {}
    for campo, padrao in regex_mappings.items():
        match = re.search(padrao, conteudo)
        dados[campo] = match.group(1) if match else None

    return dados

# Processar todos os arquivos da pasta
dados_coletados = []
for nome_arquivo in os.listdir(PASTA_RESULTADOS):
    if nome_arquivo.endswith(".txt"):
        caminho = os.path.join(PASTA_RESULTADOS, nome_arquivo)
        dados = extrair_dados_arquivo(caminho)
        
        partes = nome_arquivo.split("_")
        if len(partes) >= 3:
            palavra = partes[1]
        else:
            palavra = nome_arquivo.replace(".txt", "")
        
        dados["arquivo"] = palavra
        dados["arquivo_saida"] = nome_arquivo
        dados_coletados.append(dados)

# Transformar em DataFrame
df = pd.DataFrame(dados_coletados)

# Converter campos numéricos
colunas_numericas = [
    "total_acessos", "page_faults", "dirty_pages", "paginas_unicas",
    "substituicoes", "fault_rate", "dirty_rate", "memoria_usada_kb", "tempo_execucao"
]
for col in colunas_numericas:
    df[col] = pd.to_numeric(df[col], errors="coerce")

# Separar algoritmo e estrutura de tabela
df[["algoritmo", "tabela"]] = df["algoritmo_tabela"].str.extract(r"^([a-z]+)(?:-(.+))?$")
df["tabela"] = df["tabela"].fillna("plana")
df["arquivo"] = df["arquivo"].str.replace(".txt", "", regex=False)

# Salvar em Excel
df.to_excel(ARQUIVO_SAIDA, index=False)
print(f"✅ Resultados salvos em '{ARQUIVO_SAIDA}' com {len(df)} entradas.")
