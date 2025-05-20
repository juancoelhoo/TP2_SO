#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "pagina.h"

/**
 * Seleciona um quadro de página aleatoriamente.
 */
int substituir_pagina_random(Frame *quadros, int num_quadros);

/**
 * Seleciona a página menos recentemente usada (LRU).
 */
int substituir_pagina_lru(Frame *quadros, int num_quadros);

/**
 * Seleciona a página com menor frequência de uso (LFU).
 */
int substituir_pagina_lfu(Frame *quadros, int num_quadros);

/**
 * Seleciona a página usando o algoritmo CLOCK.
 */
int substituir_pagina_clock(Frame *quadros, int num_quadros);

#endif // ALGORITMOS_H
