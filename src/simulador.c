#include "simulador.h"
#include "util.h"
#include "pagina.h"
#include "algoritmos.h"
#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void simular(const char *algoritmo, const char *arquivo, unsigned int tam_pagina_kb, unsigned int tam_memoria_kb, int debug) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    clock_t inicio = clock();
    inicializar_simulador(tam_pagina_kb, tam_memoria_kb, algoritmo);

    unsigned addr;
    char rw;
    while (fscanf(fp, "%x %c", &addr, &rw) == 2) {
        acessar_memoria(addr, rw);
    }
    fclose(fp);
    clock_t fim = clock();

    double tempo_exec = (double)(fim - inicio) / CLOCKS_PER_SEC;
    imprimir_estatisticas(arquivo, tam_pagina_kb, tam_memoria_kb, algoritmo);
    printf("Tempo de execução: %.4f segundos\n", tempo_exec);
}
