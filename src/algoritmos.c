#include "algoritmos.h"
#include "pagina.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>

static int clock_hand = 0;

int substituir_pagina_random(Frame *quadros, int num_quadros) {
    return random() % num_quadros;
}

int substituir_pagina_lru(Frame *quadros, int num_quadros) {
    int lru_idx = -1;
    unsigned menor_tempo = UINT_MAX;
    for (int i = 0; i < num_quadros; i++) {
        if (quadros[i].ultimo_acesso < menor_tempo) {
            menor_tempo = quadros[i].ultimo_acesso;
            lru_idx = i;
        }
    }
    return lru_idx;
}

int substituir_pagina_lfu(Frame *quadros, int num_quadros) {
    int lfu_idx = -1;
    unsigned menor_freq = UINT_MAX;
    unsigned mais_antigo = UINT_MAX;
    for (int i = 0; i < num_quadros; i++) {
        if (quadros[i].frequencia < menor_freq ||
            (quadros[i].frequencia == menor_freq && quadros[i].ultimo_acesso < mais_antigo)) {
            menor_freq = quadros[i].frequencia;
            mais_antigo = quadros[i].ultimo_acesso;
            lfu_idx = i;
        }
    }
    return lfu_idx;
}

int substituir_pagina_clock(Frame *quadros, int num_quadros) {
    while (1) {
        if (quadros[clock_hand].referenciado == 0) {
            int idx = clock_hand;
            clock_hand = (clock_hand + 1) % num_quadros;
            return idx;
        }
        quadros[clock_hand].referenciado = 0;
        clock_hand = (clock_hand + 1) % num_quadros;
    }
}
