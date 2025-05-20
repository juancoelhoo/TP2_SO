
# Simulador de Memória Virtual

Este projeto implementa um simulador de memória virtual em linguagem C para o Trabalho Prático 2 da disciplina de Sistemas Operacionais da UFMG (2025/1).

## Estrutura do Projeto

```text
TP2_SO/
├── bin/                  # Arquivo executável final (simulador)
├── include/              # Arquivos de cabeçalho (.h)
├── obj/                  # Arquivos objeto (.o)
├── src/                  # Códigos-fonte principais (.c)
├── logs/                 # Arquivos de entrada com acessos de memória
├── resultados/           # Saídas geradas pelas simulações
├── graphs/               # Scripts Python para análise e visualização
├── teste_simulador.sh    # Script para rodar todas as combinações automaticamente
└── README.md             # Este documento
```

## Compilação

Use o comando abaixo na raiz do projeto:

```bash
make
```

## Execução

```bash
./bin/simulador algoritmo arquivo.log tam_pagina_kb tam_memoria_kb [debug]
```

Exemplo:

```bash
./bin/simulador lru-2n logs/matriz.log 4 1024 debug
```

## Algoritmos de Substituição Suportados
- `lru` (Least Recently Used)
- `lfu` (Least Frequently Used)
- `random`
- `clock`

## Estruturas de Tabela de Páginas Suportadas
- `plana` (padrão)
- `-2n` (hierárquica de 2 níveis)
- `-3n` (hierárquica de 3 níveis)
- `-inv` (invertida)

Use o sufixo do tipo de tabela junto ao nome do algoritmo. Exemplo: `lfu-3n`, `clock-inv`, etc.

## Executar todos os testes automaticamente

```bash
./teste_simulador.sh
```

Esse script executa todas as combinações de algoritmos, tabelas e arquivos `.log`, salvando os resultados em `resultados/`.

## Análise de Resultados com Python

1. Ative seu ambiente virtual e execute:

```bash
python graphs/extract_results_to_sheets.py
```

2. Isso gera um arquivo `resultados_simulador.xlsx` com todas as métricas extraídas.

3. Para gerar gráficos automaticamente:

```bash
python graphs/plot_graphics.py
```

## Dependências Python (para análise)

```bash
pip install pandas matplotlib seaborn openpyxl
```

## Métricas Coletadas
- Total de acessos
- Page faults
- Páginas sujas
- Páginas únicas acessadas
- Substituições
- Tempo de execução
- Uso de memória pelas tabelas
- Taxas de page fault e escrita

---

## Observações

Basta alterar os parâmetros de teste_simulador.sh, PAGE_SIZE e MEMSIZE, para observar diferentes comportamentos para diferentes entradas.