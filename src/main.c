#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

/**
 * Função principal: inicializa a simulação de memória virtual a partir dos argumentos da linha de comando.
 *
 * Uso:
 *   ./simulador algoritmo arquivo.log tam_pagina_kb tam_memoria_kb [debug]
 *
 * Onde:
 *   algoritmo        - Nome do algoritmo de substituição (ex: lru, lfu, random, clock)
 *   arquivo.log      - Caminho para o arquivo com acessos de memória
 *   tam_pagina_kb    - Tamanho da página em KB (potência de 2 entre 2 e 64)
 *   tam_memoria_kb   - Tamanho da memória física total em KB (entre 128 e 16384)
 *   debug (opcional) - Ativa o modo de depuração detalhado
 */
int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s algoritmo arquivo.log tam_pagina_kb tam_memoria_kb [debug]\n", argv[0]);
        return 1;
    }

    const char *algoritmo = argv[1];
    const char *arquivo = argv[2];
    unsigned int tam_pagina_kb = atoi(argv[3]);
    unsigned int tam_memoria_kb = atoi(argv[4]);
    int modo_debug = (argc == 6);  // Modo debug ativado se há 6 argumentos

    simular(algoritmo, arquivo, tam_pagina_kb, tam_memoria_kb, modo_debug);

    return 0;
}
