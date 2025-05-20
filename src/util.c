#include "util.h"
#include <stdarg.h>
#include <stdio.h>

static int modo_debug_ativo = 0;

/**
 * Ativa ou desativa o modo debug.
 * @param val 1 para ativar, 0 para desativar.
 */
void set_debug(int val) {
    modo_debug_ativo = val;
}

/**
 * Obtém o número da página lógica a partir de um endereço virtual e do shift calculado.
 * @param addr Endereço virtual de 32 bits.
 * @param s Quantidade de bits para deslocamento (log2 do tamanho da página).
 * @return Número da página lógica.
 */
unsigned obter_numero_pagina(unsigned addr, unsigned s) {
    return addr >> s;
}

/**
 * Calcula o valor de shift (log2 do tamanho da página em bytes).
 * @param page_size Tamanho da página em bytes (ex: 4096).
 * @return Quantidade de bits a deslocar para obter o número da página.
 */
unsigned calcular_shift(unsigned page_size) {
    unsigned tmp = page_size;
    unsigned s = 0;

    while (tmp > 1) {
        tmp >>= 1;
        s++;
    }

    return s;
}

/**
 * Imprime mensagens de depuração, se o modo debug estiver ativado.
 * Funciona como printf, usando formato variável.
 */
void debug_print(const char *fmt, ...) {
    if (modo_debug_ativo) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}
