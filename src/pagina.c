#include "pagina.h"
#include <stdlib.h>
#include <limits.h>

static Frame *quadros = NULL;     // Vetor de quadros de memória física
static int total_quadros = 0;     // Número total de quadros alocados

/**
 * Retorna o vetor de quadros ativos.
 */
Frame *obter_quadros() {
    return quadros;
}

/**
 * Inicializa todos os quadros de memória física.
 */
void inicializar_quadros(int num_quadros) {
    total_quadros = num_quadros;
    quadros = (Frame *) malloc(sizeof(Frame) * num_quadros);

    for (int i = 0; i < num_quadros; i++) {
        quadros[i].pagina = -1;
        quadros[i].modificado = 0;
        quadros[i].referenciado = 0;
        quadros[i].frequencia = 0;
        quadros[i].ultimo_acesso = 0;
    }
}

/**
 * Busca a página informada nos quadros de memória.
 * Retorna o índice do quadro, ou -1 se não estiver presente.
 */
int buscar_pagina(int pagina) {
    for (int i = 0; i < total_quadros; i++) {
        if (quadros[i].pagina == pagina) {
            return i;
        }
    }
    return -1;
}

/**
 * Retorna o índice de um quadro livre (sem página carregada), ou -1 se não houver.
 */
int obter_frame_livre() {
    for (int i = 0; i < total_quadros; i++) {
        if (quadros[i].pagina == -1) {
            return i;
        }
    }
    return -1;
}

/**
 * Marca um acesso à página no quadro especificado.
 */
void marcar_acesso(int idx, unsigned tempo, char rw) {
    quadros[idx].ultimo_acesso = tempo;
    quadros[idx].frequencia++;
    quadros[idx].referenciado = 1;

    if (rw == 'W') {
        quadros[idx].modificado = 1;
    }
}

/**
 * Carrega uma nova página no quadro indicado.
 */
void carregar_pagina(int idx, int pagina, unsigned tempo, char rw) {
    quadros[idx].pagina = pagina;
    quadros[idx].ultimo_acesso = tempo;
    quadros[idx].frequencia = 1;
    quadros[idx].referenciado = 1;
    quadros[idx].modificado = (rw == 'W');
}

/**
 * Verifica se a página no quadro foi modificada.
 */
int esta_modificado(int idx) {
    return quadros[idx].modificado;
}

/**
 * Retorna o número da página presente em um quadro.
 */
int obter_pagina(int idx) {
    return quadros[idx].pagina;
}

/**
 * Libera a memória alocada para os quadros.
 */
void desalocar_quadros() {
    if (quadros) {
        free(quadros);
        quadros = NULL;
    }
}
