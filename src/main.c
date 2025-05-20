#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s algoritmo arquivo.log tam_pagina_kb tam_memoria_kb [debug]\n", argv[0]);
        return 1;
    }

    const char *algoritmo = argv[1];
    const char *arquivo = argv[2];
    unsigned int tam_pagina_kb = atoi(argv[3]);
    unsigned int tam_memoria_kb = atoi(argv[4]);
    int modo_debug = (argc == 6);

    simular(algoritmo, arquivo, tam_pagina_kb, tam_memoria_kb, modo_debug);

    return 0;
}
