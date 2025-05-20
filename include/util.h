#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

/**
 * Retorna o número da página lógica a partir de um endereço virtual.
 */
unsigned obter_numero_pagina(unsigned addr, unsigned s);

/**
 * Calcula o shift necessário para determinar o número de página.
 */
unsigned calcular_shift(unsigned page_size);

/**
 * Imprime informações de depuração (ativado se estiver em modo debug).
 */
void debug_print(const char *fmt, ...);

#endif // UTIL_H
