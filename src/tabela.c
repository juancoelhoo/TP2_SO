#include "tabela.h"
#include "pagina.h"
#include "util.h"
#include "algoritmos.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

// Enumeração dos tipos de tabela de páginas
typedef enum {
    TABELA_PLANA,
    TABELA_2_NIVEIS,
    TABELA_3_NIVEIS,
    TABELA_INVERTIDA
} TipoTabela;

// Constantes de configuração
#define MAX_PAGINAS (1 << 20)
#define DIR_ENTRIES 1024

// Estruturas de tabelas
static int *tabela_plana;
static int **tabela_2n;
static int ***tabela_3n;

typedef struct {
    int pagina;
} EntradaInvertida;

static EntradaInvertida *tabela_invertida;

// Variáveis globais da simulação
static TipoTabela tipo_tabela = TABELA_PLANA;
static int num_quadros;
static unsigned tempo = 0;
static int page_faults = 0;
static int paginas_sujas = 0;
static unsigned paginas_unicas[MAX_PAGINAS] = {0};
static int num_substituicoes = 0;
static const char *algoritmo_nome;
static int (*substituir_pagina)(Frame *, int) = NULL;

/**
 * Inicializa simulador e estruturas de acordo com parâmetros.
 */
void inicializar_simulador(unsigned page_size_kb, unsigned mem_size_kb, const char *algoritmo_input) {
    unsigned page_size = page_size_kb * 1024;
    unsigned mem_size = mem_size_kb * 1024;
    num_quadros = mem_size / page_size;

    char algoritmo[64];
    strncpy(algoritmo, algoritmo_input, 63);
    algoritmo[63] = '\0';

    // Detecta estrutura de tabela
    if (strstr(algoritmo, "-2n")) tipo_tabela = TABELA_2_NIVEIS;
    else if (strstr(algoritmo, "-3n")) tipo_tabela = TABELA_3_NIVEIS;
    else if (strstr(algoritmo, "-inv")) tipo_tabela = TABELA_INVERTIDA;
    else tipo_tabela = TABELA_PLANA;

    char *hifen = strchr(algoritmo, '-');
    if (hifen) *hifen = '\0';

    // Seleciona algoritmo de substituição
    if (strcmp(algoritmo, "lru") == 0) substituir_pagina = substituir_pagina_lru;
    else if (strcmp(algoritmo, "lfu") == 0) substituir_pagina = substituir_pagina_lfu;
    else if (strcmp(algoritmo, "random") == 0) substituir_pagina = substituir_pagina_random;
    else if (strcmp(algoritmo, "clock") == 0) substituir_pagina = substituir_pagina_clock;
    else {
        fprintf(stderr, "Algoritmo inválido: %s\n", algoritmo_input);
        exit(1);
    }

    algoritmo_nome = algoritmo_input;
    srandom(time(NULL));

    // Inicializa estrutura de tabela
    switch (tipo_tabela) {
        case TABELA_PLANA:
            tabela_plana = malloc(sizeof(int) * MAX_PAGINAS);
            for (int i = 0; i < MAX_PAGINAS; i++) tabela_plana[i] = -1;
            break;
        case TABELA_2_NIVEIS:
            tabela_2n = calloc(DIR_ENTRIES, sizeof(int *));
            break;
        case TABELA_3_NIVEIS:
            tabela_3n = calloc(DIR_ENTRIES, sizeof(int **));
            break;
        case TABELA_INVERTIDA:
            tabela_invertida = malloc(sizeof(EntradaInvertida) * num_quadros);
            for (int i = 0; i < num_quadros; i++) tabela_invertida[i].pagina = -1;
            break;
    }

    inicializar_quadros(num_quadros);
}

/**
 * Acesso com tabela plana (vetor direto).
 */
void acessar_tabela_plana(unsigned addr, char rw) {
    unsigned s = calcular_shift(4096);
    int pagina = obter_numero_pagina(addr, s);
    tempo++;

    if (!paginas_unicas[pagina]) {
        paginas_unicas[pagina] = 1;
    }

    int idx = buscar_pagina(pagina);
    if (idx != -1) {
        marcar_acesso(idx, tempo, rw);
        return;
    }

    page_faults++;

    int livre = obter_frame_livre();
    if (livre != -1) {
        carregar_pagina(livre, pagina, tempo, rw);
        tabela_plana[pagina] = livre;
    } else {
        int substituir = substituir_pagina(obter_quadros(), num_quadros);
        num_substituicoes++;

        if (esta_modificado(substituir)) {
            paginas_sujas++;
        }

        int antiga = obter_pagina(substituir);
        tabela_plana[antiga] = -1;

        carregar_pagina(substituir, pagina, tempo, rw);
        tabela_plana[pagina] = substituir;
    }
}

/**
 * Acesso com tabela hierárquica de 2 níveis.
 */
void acessar_tabela_2n(unsigned addr, char rw) {
    unsigned s = calcular_shift(4096);
    unsigned page = obter_numero_pagina(addr, s);
    unsigned dir = (page >> 10) & 0x3FF;
    unsigned tab = page & 0x3FF;
    tempo++;
    if (!paginas_unicas[page]) paginas_unicas[page] = 1;

    if (!tabela_2n[dir]) {
        tabela_2n[dir] = malloc(sizeof(int) * DIR_ENTRIES);
        for (int i = 0; i < DIR_ENTRIES; i++) tabela_2n[dir][i] = -1;
    }

    int idx = buscar_pagina(page);
    if (idx != -1) {
        marcar_acesso(idx, tempo, rw);
        return;
    }

    page_faults++;
    int livre = obter_frame_livre();
    if (livre != -1) {
        carregar_pagina(livre, page, tempo, rw);
        tabela_2n[dir][tab] = livre;
    } else {
        int substituir = substituir_pagina(obter_quadros(), num_quadros);
        num_substituicoes++;
        if (esta_modificado(substituir)) paginas_sujas++;
        int antiga = obter_pagina(substituir);
        tabela_2n[(antiga >> 10) & 0x3FF][antiga & 0x3FF] = -1;
        carregar_pagina(substituir, page, tempo, rw);
        tabela_2n[dir][tab] = substituir;
    }
}

/**
 * Acesso com tabela hierárquica de 3 níveis.
 */
void acessar_tabela_3n(unsigned addr, char rw) {
    unsigned s = calcular_shift(4096);
    unsigned page = obter_numero_pagina(addr, s);
    unsigned lvl1 = (page >> 20) & 0x3FF;
    unsigned lvl2 = (page >> 10) & 0x3FF;
    unsigned lvl3 = page & 0x3FF;
    tempo++;
    if (!paginas_unicas[page]) paginas_unicas[page] = 1;

    if (!tabela_3n[lvl1]) {
        tabela_3n[lvl1] = calloc(DIR_ENTRIES, sizeof(int *));
    }
    if (!tabela_3n[lvl1][lvl2]) {
        tabela_3n[lvl1][lvl2] = malloc(sizeof(int) * DIR_ENTRIES);
        for (int i = 0; i < DIR_ENTRIES; i++) tabela_3n[lvl1][lvl2][i] = -1;
    }

    int idx = buscar_pagina(page);
    if (idx != -1) {
        marcar_acesso(idx, tempo, rw);
        return;
    }

    page_faults++;
    int livre = obter_frame_livre();
    if (livre != -1) {
        carregar_pagina(livre, page, tempo, rw);
        tabela_3n[lvl1][lvl2][lvl3] = livre;
    } else {
        int substituir = substituir_pagina(obter_quadros(), num_quadros);
        num_substituicoes++;
        if (esta_modificado(substituir)) paginas_sujas++;
        int antiga = obter_pagina(substituir);
        unsigned a1 = (antiga >> 20) & 0x3FF;
        unsigned a2 = (antiga >> 10) & 0x3FF;
        unsigned a3 = antiga & 0x3FF;
        tabela_3n[a1][a2][a3] = -1;
        carregar_pagina(substituir, page, tempo, rw);
        tabela_3n[lvl1][lvl2][lvl3] = substituir;
    }
}

/**
 * Acesso com tabela invertida.
 */
void acessar_tabela_invertida(unsigned addr, char rw) {
    unsigned s = calcular_shift(4096);
    int pagina = obter_numero_pagina(addr, s);
    tempo++;
    if (!paginas_unicas[pagina]) paginas_unicas[pagina] = 1;

    for (int i = 0; i < num_quadros; i++) {
        if (tabela_invertida[i].pagina == pagina) {
            marcar_acesso(i, tempo, rw);
            return;
        }
    }

    page_faults++;
    int livre = obter_frame_livre();
    if (livre != -1) {
        carregar_pagina(livre, pagina, tempo, rw);
        tabela_invertida[livre].pagina = pagina;
    } else {
        int substituir = substituir_pagina(obter_quadros(), num_quadros);
        num_substituicoes++;
        if (esta_modificado(substituir)) paginas_sujas++;
        tabela_invertida[substituir].pagina = pagina;
        carregar_pagina(substituir, pagina, tempo, rw);
    }
}

/**
 * Encaminha o acesso à função correspondente ao tipo da tabela atual.
 */
void acessar_memoria(unsigned addr, char rw) {
    switch (tipo_tabela) {
        case TABELA_PLANA:
            acessar_tabela_plana(addr, rw);
            break;
        case TABELA_2_NIVEIS:
            acessar_tabela_2n(addr, rw);
            break;
        case TABELA_3_NIVEIS:
            acessar_tabela_3n(addr, rw);
            break;
        case TABELA_INVERTIDA:
            acessar_tabela_invertida(addr, rw);
            break;
    }
}

/**
 * Calcula o uso de memória pelas estruturas de tabela de páginas.
 */
unsigned long calcular_uso_memoria() {
    switch (tipo_tabela) {
        case TABELA_PLANA:
            return sizeof(int) * MAX_PAGINAS;

        case TABELA_2_NIVEIS: {
            unsigned long total = sizeof(int *) * DIR_ENTRIES;
            for (int i = 0; i < DIR_ENTRIES; i++)
                if (tabela_2n[i]) total += sizeof(int) * DIR_ENTRIES;
            return total;
        }

        case TABELA_3_NIVEIS: {
            unsigned long total = sizeof(int **) * DIR_ENTRIES;
            for (int i = 0; i < DIR_ENTRIES; i++)
                if (tabela_3n[i])
                    for (int j = 0; j < DIR_ENTRIES; j++)
                        if (tabela_3n[i][j]) total += sizeof(int) * DIR_ENTRIES;
            return total;
        }

        case TABELA_INVERTIDA:
            return sizeof(EntradaInvertida) * num_quadros;
    }
    return 0;
}

/**
 * Imprime todas as estatísticas finais da execução da simulação.
 */
void imprimir_estatisticas(const char *arquivo, unsigned page_size_kb, unsigned mem_size_kb, const char *algoritmo) {
    printf("\nExecutando o simulador...\n\n");
    printf("Arquivo de entrada: %s\n", arquivo);
    printf("Tamanho da memória: %u KB\n", mem_size_kb);
    printf("Tamanho das páginas: %u KB\n", page_size_kb);
    printf("Técnica de reposição: %s\n\n", algoritmo);

    printf("Total de acessos: %u\n", tempo);
    printf("Páginas lidas (page faults): %d\n", page_faults);
    printf("Páginas escritas (dirty pages): %d\n", paginas_sujas);

    unsigned long mem_bytes = calcular_uso_memoria();
    unsigned total_unicos = 0;

    for (int i = 0; i < MAX_PAGINAS; i++) {
        if (paginas_unicas[i]) {
            total_unicos++;
        }
    }

    printf("Número de páginas únicas acessadas: %u\n", total_unicos);
    printf("Número de substituições de página: %d\n", num_substituicoes);
    printf("Taxa de page faults: %.2f%%\n", (page_faults * 100.0) / tempo);
    printf("Taxa de páginas sujas: %.2f%%\n", (page_faults > 0) ? (paginas_sujas * 100.0) / page_faults : 0.0);
    printf("Memória usada pelas tabelas: %lu bytes (%.2f KB)\n", mem_bytes, mem_bytes / 1024.0);
}