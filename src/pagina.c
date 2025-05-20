#include "pagina.h"
#include <stdlib.h>
#include <limits.h>

static Frame *quadros = NULL;
static int total_quadros;

Frame *obter_quadros() {
    return quadros;
}

void inicializar_quadros(int num_quadros) {
    total_quadros = num_quadros;
    quadros = (Frame *)malloc(sizeof(Frame) * num_quadros);
    for (int i = 0; i < num_quadros; i++) {
        quadros[i].pagina = -1;
        quadros[i].modificado = 0;
        quadros[i].referenciado = 0;
        quadros[i].frequencia = 0;
        quadros[i].ultimo_acesso = 0;
    }
}

int buscar_pagina(int pagina) {
    for (int i = 0; i < total_quadros; i++) {
        if (quadros[i].pagina == pagina)
            return i;
    }
    return -1;
}

int obter_frame_livre() {
    for (int i = 0; i < total_quadros; i++) {
        if (quadros[i].pagina == -1)
            return i;
    }
    return -1;
}

void marcar_acesso(int idx, unsigned tempo, char rw) {
    quadros[idx].ultimo_acesso = tempo;
    quadros[idx].frequencia++;
    quadros[idx].referenciado = 1;
    if (rw == 'W') quadros[idx].modificado = 1;
}

void carregar_pagina(int idx, int pagina, unsigned tempo, char rw) {
    quadros[idx].pagina = pagina;
    quadros[idx].ultimo_acesso = tempo;
    quadros[idx].frequencia = 1;
    quadros[idx].referenciado = 1;
    quadros[idx].modificado = (rw == 'W');
}

int esta_modificado(int idx) {
    return quadros[idx].modificado;
}

int obter_pagina(int idx) {
    return quadros[idx].pagina;
}

void desalocar_quadros() {
    if (quadros) free(quadros);
    quadros = NULL;
}
