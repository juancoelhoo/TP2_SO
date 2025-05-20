#include "simulador.h"
#include "util.h"
#include "pagina.h"
#include "algoritmos.h"
#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Executa a simulação da memória virtual com os parâmetros fornecidos.
 * 
 * @param algoritmo Nome do algoritmo de substituição de páginas (lru, lfu, random, clock)
 * @param arquivo Caminho para o arquivo de acessos de memória (.log)
 * @param tam_pagina_kb Tamanho da página em kilobytes
 * @param tam_memoria_kb Tamanho da memória física total em kilobytes
 * @param debug Ativa o modo de depuração detalhado
 */
void simular(const char *algoritmo, const char *arquivo,
             unsigned int tam_pagina_kb, unsigned int tam_memoria_kb, int debug) {
    
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    clock_t inicio = clock();

    // Inicializa estruturas internas da simulação
    inicializar_simulador(tam_pagina_kb, tam_memoria_kb, algoritmo);

    unsigned addr;
    char rw;

    // Lê os acessos de memória do arquivo (formato: <endereço_hex> <R|W>)
    while (fscanf(fp, "%x %c", &addr, &rw) == 2) {
        acessar_memoria(addr, rw);
    }

    fclose(fp);

    clock_t fim = clock();
    double tempo_exec = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Exibe estatísticas finais da simulação
    imprimir_estatisticas(arquivo, tam_pagina_kb, tam_memoria_kb, algoritmo);
    printf("Tempo de execução: %.4f segundos\n", tempo_exec);
}
